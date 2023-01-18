class Player {
    constructor(x, y, size) {
        this.position = new p5.Vector(x, y);
        this.velocity = new p5.Vector(0, 3);
        this.size = size;
        this.color = 204;
        this.key = '';
        this.changeGravity = false;
        this.hasCollided = false;
        this.catchupSpeed = 0.01;
        this.alive = true;
    }
    update(substeps) {
        if(!this.alive) return;
        this.hasCollided = false;
        this.velocity.x = this.catchupSpeed * (200 - this.position.x)
        this.position.add(p5.Vector.mult(this.velocity, 1/substeps));
    }

    checkBoundaryCollision() {
        if (this.position.x > (width)) {
            this.alive = false;
        } else if (this.position.x < - this.size) {
            this.alive = false;
        } else if (this.position.y > (height)) {
            this.alive = false;
        } else if (this.position.y < - this.size) {
            this.alive = false;
        }
    }

    checkCollision(other) {
        if(!this.alive) return;
        if (other instanceof Player) {
            if(!other.alive) return;
            if (this.position.x >= (other.position.x - this.size) && this.position.x <= (other.position.x + other.size - this.size)) {
                let penx = this.position.x - (other.position.x - this.size);
                if (this.position.y > (other.position.y - this.size) && this.position.y < (other.position.y - this.size + other.size)) {
                    let peny = this.position.y - (other.position.y - this.size);
                    if (penx < peny) {
                        this.position.x = this.position.x - penx;
                    }
                    else {
                        this.position.y = this.position.y - peny / 2;
                        other.position.y = other.position.y + peny / 2;
                        this.hasCollided = true;
                        other.hasCollided = true;
                    }

                } else if (this.position.y < (other.position.y + other.size) && this.position.y > (other.position.y + other.size - this.size)) {
                    let peny = (other.position.y + other.size) - this.position.y;
                    if (penx < peny) {
                        this.position.x = this.position.x - penx;
                    }
                    else {
                        this.position.y = this.position.y + peny / 2;
                        other.position.y = other.position.y - peny / 2;
                        this.hasCollided = true;
                        other.hasCollided = true;
                    }
                }
            } else if (this.position.x <= (other.position.x + other.size) && this.position.x >= (other.position.x + other.size - this.size)) {
                let penx = (other.position.x + other.size) - this.position.x;
                if (this.position.y > (other.position.y - this.size) && this.position.y < (other.position.y - this.size + other.size)) {
                    let peny = this.position.y - (other.position.y - this.size);
                    if (penx < peny) {
                        other.position.x = other.position.x - penx;
                    }
                    else {
                        this.position.y = this.position.y - peny / 2;
                        other.position.y = other.position.y + peny / 2;
                        this.hasCollided = true;
                        other.hasCollided = true;
                    }

                } else if (this.position.y < (other.position.y + other.size) && this.position.y > (other.position.y + other.size - this.size)) {
                    let peny = (other.position.y + other.size) - this.position.y;
                    if (penx < peny) {
                        other.position.x = other.position.x - penx;
                    }
                    else {
                        this.position.y = this.position.y + peny / 2;
                        other.position.y = other.position.y - peny / 2;
                        this.hasCollided = true;
                        other.hasCollided = true;
                    }
                }
            }
        }

        if (other instanceof Wall) {
            if (this.position.x >= (other.position.x - this.size) && this.position.x <= (other.position.x + other.sizeX - this.size)) {
                let penx = this.position.x - (other.position.x - this.size);
                if (this.position.y >= (other.position.y - this.size) && this.position.y <= (other.position.y - this.size + other.sizeY)) {
                    let peny = this.position.y - (other.position.y - this.size);
                    if (penx < peny) {
                        this.position.x = this.position.x - penx;
                    }
                    else {
                        this.position.y = this.position.y - peny;
                        this.hasCollided = true;
                    }

                } else if (this.position.y <= (other.position.y + other.sizeY) && this.position.y >= (other.position.y + other.sizeY - this.size)) {
                    let peny = (other.position.y + other.sizeY) - this.position.y;
                    if (penx < peny) {
                        this.position.x = this.position.x - penx;
                    }
                    else {
                        this.position.y = this.position.y + peny;
                        this.hasCollided = true;
                    }
                }
            } else if (this.position.x <= (other.position.x + other.sizeX) && this.position.x >= (other.position.x + other.sizeX - this.size)) {
                let penx = (other.position.x + other.sizeX) - this.position.x;
                if (this.position.y >= (other.position.y - this.size) && this.position.y <= (other.position.y - this.size + other.sizeY)) {
                    let peny = this.position.y - (other.position.y - this.size);
                    if (penx < peny) {
                        this.position.x = this.position.x + penx;
                    }
                    else {
                        this.position.y = this.position.y - peny;
                        this.hasCollided = true;
                    }

                } else if (this.position.y <= (other.position.y + other.sizeY) && this.position.y >= (other.position.y + other.sizeY - this.size)) {
                    let peny = (other.position.y + other.sizeY) - this.position.y;
                    if (penx < peny) {
                        this.position.x = this.position.x + penx;
                    }
                    else {
                        this.position.y = this.position.y + peny;
                        this.hasCollided = true;
                    }
                }
            }
        }
    }

    checkFinish(finish) {
        if(!this.alive) return false;
        if (this.position.x > finish.position.x) {
            return true;
        }
        return false;
    }

    display() {
        if(!this.alive) return;
        noStroke();
        fill(this.color);
        rect(this.position.x, this.position.y, this.size, this.size);

        stroke(100);
        fill(230);
        textSize(24);
        text(this.key, this.position.x + this.size / 2, this.position.y + this.size / 2);

    }

    checkGravity() {
        if(!this.alive) return;
        if (this.changeGravity && this.hasCollided) {
            this.velocity.y = -this.velocity.y;
        }
        this.changeGravity = false;
    }

}