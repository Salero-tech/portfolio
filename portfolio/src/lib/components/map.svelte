<script>
  let vw = $state(800);
  let vh = $state(600);

  // Each node's position + size on the big canvas.
  // Note these are NOT in a straight line — that's what makes the
  // camera move in different directions as you step through them.
  const nodes = [
    { id: 'a', x: 100,  y: 400, w: 320, h: 200, label: 'A — start' },
    { id: 'b', x: 700,  y: 100, w: 320, h: 200, label: 'B — up + right' },
    { id: 'c', x: 1300, y: 450, w: 320, h: 200, label: 'C — down + right' },
    { id: 'd', x: 750,  y: 800, w: 320, h: 200, label: 'D — down + left' },
  ];

  let index = $state(0);
  let node = $derived(nodes[index]);

  function go(i) {
    index = Math.max(0, Math.min(nodes.length - 1, i));
  }

  // Wheel/trackpad: treat ANY scroll direction (vertical or horizontal)
  // as one step forward/back, with a cooldown so a single scroll
  // gesture doesn't fire 10 steps at once.
  let locked = false;
  function handleWheel(e) {
    e.preventDefault();
    if (locked) return;
    const delta = Math.abs(e.deltaY) > Math.abs(e.deltaX) ? e.deltaY : e.deltaX;
    if (Math.abs(delta) < 6) return;
    locked = true;
    go(index + (delta > 0 ? 1 : -1));
    setTimeout(() => (locked = false), 700);
  }

  function handleKey(e) {
    if (['ArrowRight', 'ArrowDown'].includes(e.key)) go(index + 1);
    if (['ArrowLeft', 'ArrowUp'].includes(e.key)) go(index - 1);
  }

  // Touch swipe: same idea, direction-agnostic.
  let tsx = 0, tsy = 0;
  function touchStart(e) {
    tsx = e.touches[0].clientX;
    tsy = e.touches[0].clientY;
  }
  function touchEnd(e) {
    const dx = e.changedTouches[0].clientX - tsx;
    const dy = e.changedTouches[0].clientY - tsy;
    if (Math.abs(dx) < 40 && Math.abs(dy) < 40) return;
    if (Math.abs(dy) > Math.abs(dx)) go(index + (dy < 0 ? 1 : -1));
    else go(index + (dx < 0 ? 1 : -1));
  }

  // --- The camera math ---
  // scale: zoom so the focused node fills a comfortable chunk of the screen
  // tx/ty: translate so the node's CENTER lands on the viewport's CENTER
  let scale = $derived(Math.min((vw * 0.6) / node.w, (vh * 0.5) / node.h, 1.4));
  let tx = $derived(vw / 2 - (node.x + node.w / 2) * scale);
  let ty = $derived(vh / 2 - (node.y + node.h / 2) * scale);
</script>

<svelte:window
  bind:innerWidth={vw}
  bind:innerHeight={vh}
  onkeydown={handleKey}
  onwheel={handleWheel}
/>

<div
  class="viewport"
  ontouchstart={touchStart}
  ontouchend={touchEnd}
>
  <div
    class="camera"
    style="transform: translate({tx}px, {ty}px) scale({scale})"
  >
    {#each nodes as n, i}
      <div
        class="node"
        class:active={i === index}
        style="left:{n.x}px; top:{n.y}px; width:{n.w}px; height:{n.h}px;"
      >
        <h2>{n.label}</h2>
        <p>Node {i + 1} of {nodes.length}</p>
      </div>
    {/each}
  </div>

  <div class="hud">
    <button onclick={() => go(index - 1)} disabled={index === 0}>‹</button>
    <span>{index + 1} / {nodes.length}</span>
    <button onclick={() => go(index + 1)} disabled={index === nodes.length - 1}>›</button>
  </div>
</div>

<style>
  :global(html, body) {
    margin: 0;
    height: 100%;
    overflow: hidden; /* we handle "scrolling" ourselves */
    background: #111;
  }

  .viewport {
    position: fixed;
    inset: 0;
    overflow: hidden;
  }

  .camera {
    position: absolute;
    top: 0;
    left: 0;
    transform-origin: 0 0;
    transition: transform 0.7s cubic-bezier(.22, .61, .36, 1);
    will-change: transform;
  }

  .node {
    position: absolute;
    background: #1c1c1c;
    border: 2px solid #444;
    border-radius: 8px;
    padding: 20px;
    color: #eee;
    font-family: sans-serif;
    opacity: 0.35;
    transition: opacity 0.5s, border-color 0.5s;
  }
  .node.active {
    opacity: 1;
    border-color: #6cf;
  }

  .hud {
    position: fixed;
    bottom: 24px;
    left: 50%;
    transform: translateX(-50%);
    display: flex;
    align-items: center;
    gap: 14px;
    background: rgba(0, 0, 0, 0.6);
    padding: 8px 16px;
    border-radius: 20px;
    color: #eee;
    font-family: sans-serif;
  }
  .hud button {
    width: 32px;
    height: 32px;
    border-radius: 50%;
    border: 1px solid #555;
    background: #222;
    color: #eee;
    cursor: pointer;
  }
  .hud button:disabled {
    opacity: 0.3;
  }
</style>