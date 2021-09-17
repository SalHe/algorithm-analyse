// 这是用暴力算法来求解题目的，主要是为了可以方便测试
// 可以使用nodejs或Chrome来运行本代码

let prices = [1755, 1749, 1724, 1699, 1671, 1693, 1799, 1750, 1690, 1700, 1699, 1635, 1620, 1619, 1548, 1570, 1625, 1665, 1595, 1596, 1586, 1558, 1622, 1618, 1658, 1670, 1669, 1633, 1634, 1662];
let max = 0;
const p = [0, 0];
for (let i = 0; i < prices.length; i++) {
    for (let j = i + 1; j < prices.length; j++) {
        let ret = prices[j] - prices[i];
        if (ret > max) {
            max = ret;
            p[0] = i;
            p[1] = j;
        }
        console.log(ret);
    }
}
console.log(`max=${max}, [${p}]`)