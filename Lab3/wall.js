class Wall {
    constructor(x, y, sizeX, sizeY) {
        this.position = new p5.Vector(x, y);
        this.sizeX = sizeX;
        this.sizeY = sizeY;
    }

    update(speed) {
        this.position.add(speed);
    }

    display() {
        noStroke();
        fill(150);
        rect(this.position.x, this.position.y, this.sizeX, this.sizeY);
    }

}