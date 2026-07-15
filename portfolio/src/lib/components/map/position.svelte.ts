const nodes = [
    { id: 'a', x: 100,  y: 400, w: 1280, h: 800, label: 'A — start' },
    { id: 'b', x: 1400,  y: 100, w: 320, h: 200, label: 'B — up + right' },
    { id: 'c', x: 2600, y: 450, w: 320, h: 200, label: 'C — down + right' },
    { id: 'd', x: 1500,  y: 800, w: 320, h: 200, label: 'D — down + left' },
];


const pos = $state({
    index: 0,
    maxIndex: nodes.length - 1,
    vw: 10000,
    vh: 10000,
    get target() {
        return nodes[this.index];
    }
});

function movePosBy(delta: number) {
    if (pos.index + delta >= pos.maxIndex) {
        pos.index = pos.maxIndex;
    }
    else if (pos.index + delta <= 0) {
        pos.index = 0;
    }
    else {
        pos.index += delta;
    }
}





export { pos, movePosBy, nodes };