class Finish {
    constructor(x, y, sizeX, sizeY) {
        this.position = new p5.Vector(x, y);
        this.sizeX = sizeX;
        this.sizeY = sizeY;
        this.color = 100;
    }

    update(speed) {
        this.position.add(speed);
    }

    display() {
        noStroke();
        fill(this.color);
        rect(this.position.x, this.position.y, this.sizeX, this.sizeY);
    }
}