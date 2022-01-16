function cal(n, x, y) {
    var res = 0;
    for (var i = 2; i <= n; i++) {
        res = (res + y) % i;
    }
    return (res + x - 1) % n + 1;
}


function getPoints(n) {
    var points = [];
    for (var x = 1; x <= n; x++) {
        for (var y = 1; y <= n; y++) {
            points.push([
                y,
                cal(n, x, y),
                x,
            ]);
        }
    }
    return points;
}

var count = parseInt(prompt("请输入 n 的值", 5));

var dataPoints = getPoints(count);
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
        text: '加里森的任务'
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
        pointFormat: 'x = {point.z}, y = {point.x}, ans = {point.y}'
    },
    yAxis: {
        min: 0,
        max: count,
        title: null
    },
    xAxis: {
        min: 0,
        max: count,
        gridLineWidth: 1
    },
    zAxis: {
        min: 0,
        max: count,
        showFirstLabel: false
    },
    legend: {
        enabled: false
    },
    series: [{
        name: 'Data',
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
