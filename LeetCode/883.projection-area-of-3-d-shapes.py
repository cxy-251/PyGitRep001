#
# @lc app=leetcode id=883 lang=python3
#
# [883] Projection Area of 3D Shapes
#
# https://leetcode.com/problems/projection-area-of-3d-shapes/description/
#
# algorithms
# Easy (73.88%)
# Likes:    615
# Dislikes: 1436
# Total Accepted:    65.3K
# Total Submissions: 88.1K
# Testcase Example:  '[[1,2],[3,4]]'
#
# You are given an n x n grid where we place some 1 x 1 x 1 cubes that are
# axis-aligned with the x, y, and z axes.
# 
# Each value v = grid[i][j] represents a tower of v cubes placed on top of the
# cell (i, j).
# 
# We view the projection of these cubes onto the xy, yz, and zx planes.
# 
# A projection is like a shadow, that maps our 3-dimensional figure to a
# 2-dimensional plane. We are viewing the "shadow" when looking at the cubes
# from the top, the front, and the side.
# 
# Return the total area of all three projections.
# 
# 
# Example 1:
# 
# 
# Input: grid = [[1,2],[3,4]]
# Output: 17
# Explanation: Here are the three projections ("shadows") of the shape made
# with each axis-aligned plane.
# 
# 
# Example 2:
# 
# 
# Input: grid = [[2]]
# Output: 5
# 
# 
# Example 3:
# 
# 
# Input: grid = [[1,0],[0,2]]
# Output: 8
# 
# 
# 
# Constraints:
# 
# 
# n == grid.length == grid[i].length
# 1 <= n <= 50
# 0 <= grid[i][j] <= 50
# 
# 
#

# @lc code=start
class Solution:
    def projectionArea(self, grid: List[List[int]]) -> int:
        n = len(grid)
        xy = 0  # 俯视图（xy平面）
        yz = 0  # 正视图（yz平面）
        zx = 0  # 侧视图（zx平面）
        for i in range(n):
            max_row = 0
            max_col = 0
            for j in range(n):
                if grid[i][j] > 0:
                    xy += 1
                max_row = max(max_row, grid[i][j])
                max_col = max(max_col, grid[j][i])
            yz += max_row
            zx += max_col
        return xy + yz + zx

# @lc code=end

