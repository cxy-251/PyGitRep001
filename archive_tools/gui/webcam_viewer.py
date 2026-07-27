"""使用 OpenCV 显示摄像头画面；按 q 或 Esc 退出。"""

from __future__ import annotations

import argparse


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--camera", type=int, default=0)
    parser.add_argument("--width", type=int)
    parser.add_argument("--height", type=int)
    args = parser.parse_args()
    try:
        import cv2
    except ImportError as exc:
        raise SystemExit("Install opencv-python") from exc

    capture = cv2.VideoCapture(args.camera)
    if args.width:
        capture.set(cv2.CAP_PROP_FRAME_WIDTH, args.width)
    if args.height:
        capture.set(cv2.CAP_PROP_FRAME_HEIGHT, args.height)
    if not capture.isOpened():
        raise SystemExit(f"Unable to open camera {args.camera}")
    try:
        while True:
            ok, frame = capture.read()
            if not ok:
                return 1
            cv2.imshow("Webcam Viewer", frame)
            key = cv2.waitKey(1) & 0xFF
            if key in (ord("q"), 27):
                break
    finally:
        capture.release()
        cv2.destroyAllWindows()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
