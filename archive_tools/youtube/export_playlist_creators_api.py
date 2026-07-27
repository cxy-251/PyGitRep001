"""使用 YouTube Data API 导出指定播放列表中的视频和频道信息。"""

from __future__ import annotations

import argparse
import csv
from pathlib import Path

SCOPES = ["https://www.googleapis.com/auth/youtube.readonly"]


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("playlist_id")
    parser.add_argument("--client-secrets", type=Path, required=True)
    parser.add_argument("--token", type=Path, default=Path("youtube-token.json"))
    parser.add_argument("--output", type=Path, default=Path("playlist_creators.csv"))
    return parser


def get_credentials(client_secrets: Path, token_path: Path):
    try:
        from google.auth.transport.requests import Request
        from google.oauth2.credentials import Credentials
        from google_auth_oauthlib.flow import InstalledAppFlow
    except ImportError as exc:
        raise SystemExit(
            "Install google-api-python-client google-auth-oauthlib google-auth-httplib2"
        ) from exc

    credentials = None
    if token_path.exists():
        credentials = Credentials.from_authorized_user_file(str(token_path), SCOPES)
    if credentials and credentials.expired and credentials.refresh_token:
        credentials.refresh(Request())
    elif not credentials or not credentials.valid:
        flow = InstalledAppFlow.from_client_secrets_file(str(client_secrets), SCOPES)
        credentials = flow.run_local_server(port=0)
    token_path.write_text(credentials.to_json(), encoding="utf-8")
    return credentials


def main() -> int:
    args = build_parser().parse_args()
    try:
        from googleapiclient.discovery import build
    except ImportError as exc:
        raise SystemExit("Install google-api-python-client") from exc

    credentials = get_credentials(
        args.client_secrets.expanduser().resolve(),
        args.token.expanduser().resolve(),
    )
    youtube = build("youtube", "v3", credentials=credentials)
    rows: list[dict[str, str]] = []
    page_token = None
    while True:
        response = youtube.playlistItems().list(
            part="snippet",
            playlistId=args.playlist_id,
            maxResults=50,
            pageToken=page_token,
        ).execute()
        for item in response.get("items", []):
            snippet = item.get("snippet", {})
            resource = snippet.get("resourceId", {})
            rows.append(
                {
                    "video_id": resource.get("videoId", ""),
                    "video_title": snippet.get("title", ""),
                    "channel_id": snippet.get("videoOwnerChannelId", ""),
                    "channel_title": snippet.get("videoOwnerChannelTitle", ""),
                }
            )
        page_token = response.get("nextPageToken")
        if not page_token:
            break

    fields = ["video_id", "video_title", "channel_id", "channel_title"]
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="", encoding="utf-8-sig") as file:
        writer = csv.DictWriter(file, fieldnames=fields)
        writer.writeheader()
        writer.writerows(rows)
    print(f"Wrote {len(rows)} playlist items to {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
