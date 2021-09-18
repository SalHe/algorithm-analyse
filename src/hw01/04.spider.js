const https = require('https');

const stockCode = 'sh600519';
const ID_DATE = 0;
const ID_PRICE = 2;

https.get(`https://proxy.finance.qq.com/ifzqgtimg/appstock/app/newfqkline/get?_var=kline_dayqfq&param=${stockCode},day,,,320,qfq&r=${Math.random()}`, response => {
    let data = '';
    response.on('data', chunk => data += chunk);
    response.on('end', () => {
        data = /kline_dayqfq=(.+)/.exec(data)[1];
        handleStockJson(data);
    });
}).on('error', err => console.log(`访问股票数据失败！${err.message}`));

function howToBuy(prices) {
    const p = [0, 0, 0];
    for (let i = 0; i < prices.length; i++) {
        for (let j = i + 1; j < prices.length; j++) {
            let ret = prices[j] - prices[i];
            if (ret > p[2]) {
                p[0] = i;
                p[1] = j;
                p[2] = ret;
            }
        }
    }
    return p;
}

function handleStockJson(json) {
    const result = JSON.parse(json);
    const stocks = result['data'][stockCode]['qfqday'];

    const prices = [];
    const dates = [];

    const now = new Date();
    now.setHours(0, 0, 0, 0);

    stocks
        .filter(x => {
            const date = new Date(x[ID_DATE]);
            date.setMonth(date.getMonth() + 3);
            return date > now;
        }).forEach(s => {
            prices.push(parseFloat(s[ID_PRICE]));
            dates.push(s[ID_DATE]);
        });

    const how = howToBuy(prices);

    console.log(`// 本头文件由脚本自动生成`);
    console.log(`// 生成时间：${new Date().toLocaleString()}`);
    console.log();
    console.log(`#define ALL_STOCKS {${prices.join(',')}}`);
    console.log(`#define ALL_STOCKS_DATE {${dates.map(it => `"${it}"`).join(',')}}`);
    console.log(`#define STOCK_START ${how[0]}`);
    console.log(`#define STOCK_END ${how[1]}`);
    console.log(`#define STOCK_BENEFIT ${how[2]}`);
}