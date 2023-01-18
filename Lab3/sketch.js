let width = 720, height = 480;
let walls = [new Wall(0, 0, 1000, 40), new Wall(0, height - 40, 1000, 40), new Wall(300, 220, 300, 40), new Wall(700, 130, 300, 40), new Wall(700, height - 170, 300, 40),
new Wall(1100, 70, 500, 40), new Wall(1100, height - 110, 500, 40), new Wall(1750, 70, 500, 40), new Wall(1750, height - 110, 500, 40), new Wall(2200, 220, 300, 40),
new Wall(2600, 110, 300, 40), new Wall(2600, height - 150, 300, 40), new Wall(3100, 0, 400, 40), new Wall(3100, height - 40, 400, 40),
new Wall(3500, 0, 40, 100), new Wall(3500, height - 100, 40, 100), new Wall(3540, 60, 500, 40), new Wall(3540, height - 100, 500, 40),
new Wall(4100, 0, 400, 40), new Wall(4800, height - 40, 400, 40), new Wall(5500, 0, 400, 40), new Wall(6200, height - 40, 400, 40),
new Wall(6750, 150, 600, 40), new Wall(6750, height - 190, 600, 40), new Wall(7350, 150, 40, 180)];
let finish = new Finish(7250, 190, 100, 100);
let players = [new Player(100, 100, 60), new Player(100, 320, 60)];
let gameStarted = false;
let winningPlayer = 0;
let gameEnded = false;


function setup() {
    createCanvas(width, height);
    let colors = [color(200, 50, 50), color(50, 200, 50), color(50, 50, 200), color(100, 100, 50)]
    let keys = ['W', 'K', 'Up', 'Num0']
    textAlign(CENTER, CENTER);
    for (let i = 0; i < players.length; i++) {
        let p = players[i];
        p.color = colors[i];
        p.key = keys[i];
    }
    players[1].velocity.y = -players[1].velocity.y;
    finish.color = color(249, 215, 25);
}



function draw() {
    background(51);
    let translateSpeed = new p5.Vector(-3, 0);
    let substeps = 6;
    if (gameStarted && !gameEnded) {
        for (let substep = 0; substep < substeps; substep++) {
            //translate objects
            for (let i = 0; i < walls.length; i++) {
                let w = walls[i];
                w.update(p5.Vector.mult(translateSpeed, 1 / substeps));
            }

            finish.update(p5.Vector.mult(translateSpeed, 1 / substeps));

            for (let i = 0; i < players.length; i++) {
                let p = players[i];
                p.update(substeps);
                p.checkBoundaryCollision();
            }

            //check collision
            for (let i = 0; i < players.length; i++) {
                for (let j = 0; j < walls.length; j++) {
                    players[i].checkCollision(walls[j]);
                }
                for (let j = i + 1; j < players.length; j++) {
                    players[i].checkCollision(players[j]);
                }
            }
        }

        let allPlayersDead = true;
        for (let i = 0; i < players.length; i++) {
            let p = players[i];
            if(p.alive) {
                allPlayersDead = false;
            }
        }
        if(allPlayersDead) {
            gameEnded = true;
            winningPlayer = -1;
        }

        for (let i = 0; i < players.length; i++) {
            let p = players[i];
            if (p.checkFinish(finish)) {
                gameEnded = true;
                winningPlayer = i;
            }
        }

        //check keys
        for (let i = 0; i < players.length; i++) {
            let p = players[i];
            p.checkGravity();
        }
    }


    //display
    for (let i = 0; i < walls.length; i++) {
        let w = walls[i];
        w.display();
    }

    finish.display();

    for (let i = 0; i < players.length; i++) {
        let p = players[i];
        p.display();
    }

    if (!gameStarted) {
        fill(255);
        textSize(40);
        text("Press space to start", width / 2, height / 2);
    }

    if (gameEnded) {
        fill(255);
        textSize(40);
        if(winningPlayer != -1){
            text("Player " + (winningPlayer + 1).toString() + " has won", width / 2, height / 2);
        }
        else {
            text("No player has reached the finish line", width / 2, height / 2);
        }
    }
}

function keyPressed() {
    console.log(key);
    switch (key) {
        case 'w':
            if (players.length >= 1)
                players[0].changeGravity = true;
            break;
        case 'k':
            if (players.length >= 2)
                players[1].changeGravity = true;
            break;
        case 'ArrowUp':
            if (players.length >= 3)
                players[2].changeGravity = true;
            break;
        case '0':
            if (players.length >= 4)
                players[3].changeGravity = true;
            break;
        case ' ':
            gameStarted = true;
            break;
    };
}