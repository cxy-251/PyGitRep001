/*
 * @lc app=leetcode id=71 lang=javascript
 *
 * [71] Simplify Path
 */

// @lc code=start
/**
 * @param {string} path
 * @return {string}
 */
var simplifyPath = function(path) {
    // 1. 使用 '/' 分割路径，得到各个组件的数组
    const components = path.split('/');
    // 2. 创建一个栈，用于存储最终的目录名
    const stack = [];

    // 3. 遍历每一个组件
    for (let component of components) {
        if (component === '..') {
            // 如果是 '..'，弹出栈顶元素（回到上一级），前提是栈不为空
            if (stack.length > 0) {
                stack.pop();
            }
        } else if (component === '.' || component === '') {
            // 如果是 '.' 或者为空字符串（由 // 产生），直接忽略
            continue;
        } else {
            // 如果是正常的目录名，入栈
            stack.push(component);
        }
    }

    // 4. 将栈中的元素用 '/' 拼接，并在最前面补上 '/'
    return '/' + stack.join('/');
};
// @lc code=end

