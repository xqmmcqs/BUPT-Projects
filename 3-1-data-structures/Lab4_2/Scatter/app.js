/**
 * 可视化实现说明：
 *  通过 raw 数组存放原始数据。对于每个元素，第一维是字符串长度，第二维是字符集大小，第三维是压缩率。
 *  由于字符串长度取值范围较大，使用 logarithmic函数，对每个元素的 x 坐标取以 10 为底的对数。
 * 另外对 x,z 维度保留两位小数。以便直观显示图像。通过对 mousemove，mouseup，mousedown 等事件的绑定，
 * 实现鼠标拖拽旋转。
 * 
 */
const raw = [
    [10, 3, 2.1000000],
    [10, 6, 2.88],
    [10, 10, 3.29],
    [10, 20, 3.7700000],
    [10, 30, 3.8899999],
    [10, 50, 4.3100000],
    [10, 95, 4.21],
    [30, 3, 0.8400000],
    [30, 6, 1.2533333],
    [30, 10, 1.7466666],
    [30, 20, 2.3933333],
    [30, 30, 2.7833333],
    [30, 50, 3.0366666],
    [30, 95, 3.5066666],
    [50, 3, 0.588],
    [50, 6, 0.8900000],
    [50, 10, 1.2200000],
    [50, 20, 1.8279999],
    [50, 30, 2.268],
    [50, 50, 2.7300000],
    [50, 95, 3.214],
    [100, 3, 0.396],
    [100, 6, 0.6090000],
    [100, 10, 0.8100000],
    [100, 20, 1.225],
    [100, 30, 1.5739999],
    [100, 50, 2.029],
    [100, 95, 2.6369999],
    [500, 3, 0.2448],
    [500, 6, 0.3854],
    [500, 10, 0.4998],
    [500, 20, 0.6816],
    [500, 30, 0.8130000],
    [500, 50, 1.0222000],
    [500, 95, 1.3958],
    [1000, 3, 0.2262000],
    [1000, 6, 0.3582999],
    [1000, 10, 0.4607],
    [1000, 20, 0.6142],
    [1000, 30, 0.7144],
    [1000, 50, 0.8658000],
    [1000, 95, 1.1122],
    [10000, 3, 0.22582],
    [10000, 6, 0.3586499],
    [10000, 10, 0.46097],
    [10000, 20, 0.61383],
    [10000, 30, 0.71445],
    [10000, 50, 0.8661000],
    [10000, 95, 1.1120400],
    [1000000, 3, 0.2253909],
    [1000000, 6, 0.3580869],
    [1000000, 10, 0.4600915],
    [1000000, 20, 0.6123894],
    [1000000, 30, 0.7120551],
    [1000000, 50, 0.8623438],
    [1000000, 95, 1.1053311],
]
const logarithmic = (raw) => {
    const ret = [];
    for (let i = 0; i < raw.length; i++) {
        const el = raw[i];
        ret.push([Math.log10(el[0]).toFixed(2), el[1], el[2].toFixed(2)]);
    }
    return ret;
}

var dataPoints = logarithmic(raw);
// Give the points a 3D feel by adding a radial gradient
Highcharts.setOptions({
    colors: Highcharts.getOptions().colors.map(function (color) {
        return {
            radialGradient: {
                cx: 0.4,
                cy: 0.3,
                r: 0.5
            },
            stops: [
                [0, color],
                [1, Highcharts.color(color).brighten(-0.2).get('rgb')]
            ]
        };
    })
});

var chart = new Highcharts.Chart({
    chart: {
        renderTo: 'container',
        margin: 100,
        type: 'scatter3d',
        animation: false,
        options3d: {
            enabled: true,
            alpha: 10,
            beta: 30,
            depth: 250,
            viewDistance: 5,
            fitToPlot: false,
            frame: {
                bottom: {
                    size: 1,
                    color: 'rgba(0,0,0,0.02)'
                },
                back: {
                    size: 1,
                    color: 'rgba(0,0,0,0.04)'
                },
                side: {
                    size: 1,
                    color: 'rgba(0,0,0,0.06)'
                }
            }
        }
    },
    title: {
        text: '哈夫曼编码压缩'
    },
    subtitle: {
        text: '在不同条件下的结果对比，拖动图表以旋转'
    },
    plotOptions: {
        scatter: {
            width: 19,
            height: 19,
            depth: 19
        }
    },
    tooltip: {
        pointFormat: '字符串长度：10^{point.x}<br>字符集大小：{point.y} <br>压缩率：{point.z}'
    },
    yAxis: {
        min: 0,
        max: 100,
        title: { text: "字符集大小" }
    },
    xAxis: {
        min: 0,
        max: 6,
        title: { text: "字符串长度" }
    },
    zAxis: {
        title: { text: "压缩率" },
        min: 0,
        max: 5,
    },
    legend: {
        enabled: false
    },
    series: [{
        name: '数据点',
        colorByPoint: true,
        accessibility: {
            exposeAsGroupOnly: true
        },
        data: dataPoints
    }]
});


(function (H) {
    function dragStart(eStart) {
        eStart = chart.pointer.normalize(eStart);

        var posX = eStart.chartX,
            posY = eStart.chartY,
            alpha = chart.options.chart.options3d.alpha,
            beta = chart.options.chart.options3d.beta,
            sensitivity = 5,
            handlers = [];

        function drag(e) {
            e = chart.pointer.normalize(e);

            chart.update({
                chart: {
                    options3d: {
                        alpha: alpha + (e.chartY - posY) / sensitivity,
                        beta: beta + (posX - e.chartX) / sensitivity
                    }
                }
            }, undefined, undefined, false);
        }

        function unbindAll() {
            handlers.forEach(function (unbind) {
                if (unbind) {
                    unbind();
                }
            });
            handlers.length = 0;
        }

        handlers.push(H.addEvent(document, 'mousemove', drag));
        handlers.push(H.addEvent(document, 'touchmove', drag));


        handlers.push(H.addEvent(document, 'mouseup', unbindAll));
        handlers.push(H.addEvent(document, 'touchend', unbindAll));
    }
    H.addEvent(chart.container, 'mousedown', dragStart);
    H.addEvent(chart.container, 'touchstart', dragStart);
}(Highcharts));
