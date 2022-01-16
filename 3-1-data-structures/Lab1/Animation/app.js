/**
* C'est un simulateur de "Garrison's Gorillas"
* @copyright Zhang Zijing <i@pluvet.com>
*/


const simulator = {
    store: {
        canvas: null,
        n: 10, // count of expandables
        x: 1, // start number of soldier
        y: 3, // flag number
        circles: null
    },
    util: {
    },
    start: () => {

        var currentNode = simulator.store.circles.getNodeAtIndex(simulator.store.x - 1);
        var flag = 1;
        var ticker = setInterval(() => {
            console.log("now at ", currentNode);
            console.log("length: ", simulator.store.circles.length);

            currentNode.prev.value.graphics.clear().beginFill(currentNode.prev.value._id == 0 ? "#009fff" : "#00bfff").drawCircle(0, 0, 20);
            currentNode.value.graphics.clear().beginFill("#ff7777").drawCircle(0, 0, 20);

            if (flag == simulator.store.y) {
                console.log("over");
                currentNode.value.graphics.clear().beginFill("#000").drawCircle(0, 0, 20);
                currentNode = simulator.store.circles.remove(currentNode);
                flag = 0;
            } else {
                currentNode = currentNode.next;
            }
            flag++;

            if (simulator.store.circles.length == 1) {
                clearInterval(ticker);
            }

        }, 100);
    }
}



function init() {


    var n = parseInt(prompt("请输入人数 (n)：", 10));
    var x = parseInt(prompt("请输入开始编号 (x)：", 1));
    var y = parseInt(prompt("请输入选取间隔：(y)", 3));

    if (n >= 40) {
        alert("人太多了坐不下了！")
    }

    if (n <= 1) {
        alert("？？？");
    }

    x = Math.abs(x) % n;

    simulator.store.n = n;
    simulator.store.x = x;
    simulator.store.y = y;

    simulator.store.canvas = document.getElementById("demoCanvas");
    simulator.store.circles = new DoublyLinkedList();

    console.log("init");
    var stage = new createjs.Stage("demoCanvas");

    var deltaDegree = 2 * Math.PI / simulator.store.n;
    var radius = 240;
    var center = {};
    center.x = simulator.store.canvas.width / 2;
    center.y = simulator.store.canvas.height / 2;

    for (let i = 0; i < simulator.store.n; i++) {

        var circle = new createjs.Shape();
        circle.graphics.beginFill(i == 0 ? "#009fff" : "#00bfff").drawCircle(0, 0, 20);
        circle.x = center.x + radius * Math.cos(deltaDegree * i);
        circle.y = center.y + radius * Math.sin(deltaDegree * i);
        circle._id = i;
        simulator.store.circles.push(circle);
        stage.addChild(circle);

        var text = new createjs.Text(circle._id + 1, "20px Arial", "#fff");
        text.x = center.x + radius * Math.cos(deltaDegree * i);
        text.y = center.y + radius * Math.sin(deltaDegree * i);
        text.textAlign = "center"
        text.textBaseline = "middle";
        stage.addChild(text);
    }
    simulator.store.circles.getNodeAtIndex(simulator.store.n - 1).next = simulator.store.circles.getNodeAtIndex(0);
    simulator.store.circles.getNodeAtIndex(0).prev = simulator.store.circles.getNodeAtIndex(simulator.store.n - 1);

    createjs.Ticker.setFPS(60);
    createjs.Ticker.addEventListener("tick", stage);

    simulator.start();

}

