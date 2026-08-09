
/**
 * @param {string} path
 * @return {string}
 */
var simplifyPath = function(path) {
    const components = path.split('/');
    const stack = [];
    for (let component of components) {
        if (component === '..') {
            if (stack.length > 0) stack.pop();
        } else if (component === '.' || component === '') {
            continue;
        } else {
            stack.push(component);
        }
    }
    return '/' + stack.join('/');
};

// --- 问题 2: Two Sum (假设的第二个问题) ---
var twoSum = function(nums, target) {
    const map = new Map();
    for (let i = 0; i < nums.length; i++) {
        const complement = target - nums[i];
        if (map.has(complement)) return [map.get(complement), i];
        map.set(nums[i], i);
    }
};
// --- 自测试部分 ---
const input = "/a/./b/../../c/"; 
console.log("输入:", input);
console.log("结果:", simplifyPath(input));
