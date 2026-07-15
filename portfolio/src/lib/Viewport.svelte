<script>
    import PCB from '$lib/PCB/PCB.svelte';
    import Hud from './hud.svelte';
    import { pos, movePosBy } from '$lib/position.svelte';


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
    movePosBy((delta > 0 ? 1 : -1));
    setTimeout(() => (locked = false), 200);
  }

  function handleKey(e) {
    if (['ArrowRight', 'ArrowDown'].includes(e.key)) movePosBy(1);
    if (['ArrowLeft', 'ArrowUp'].includes(e.key)) movePosBy(-1);
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
    if (Math.abs(dy) > Math.abs(dx)) movePosBy(dy < 0 ? 1 : -1);
    else movePosBy(dx < 0 ? 1 : -1);
  }

  // --- The camera math ---
  // scale: zoom so the focused node fills a comfortable chunk of the screen
  // tx/ty: translate so the node's CENTER lands on the viewport's CENTER
  let scale = $derived(Math.min((pos.vw * 0.6) / pos.target.w, (pos.vh * 0.5) / pos.target.h, 1.4));
  let tx = $derived(pos.vw / 2 - (pos.target.x + pos.target.w / 2) * scale);
  let ty = $derived(pos.vh / 2 - (pos.target.y + pos.target.h / 2) * scale);
</script>

<svelte:window
  bind:innerWidth={pos.vw}
  bind:innerHeight={pos.vh}
  onkeydown={handleKey}
  onwheel={handleWheel}
/>

<!-- svelte-ignore a11y_no_static_element_interactions -->
<div
  class="viewport"
  ontouchstart={touchStart}
  ontouchend={touchEnd}
>
  <div class="camera" style="transform: translate({tx}px, {ty}px) scale({scale})">
  <PCB />
  </div>
  <Hud />
</div>

<style>
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
</style>