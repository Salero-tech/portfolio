<script lang="ts">
    import { pos, movePosBy } from '$lib/position.svelte';
</script>


<div class="hud">
    <button onclick={() => movePosBy(-1)} disabled={pos.index === 0}>‹</button>
    <span>{pos.index + 1} / {pos.maxIndex + 1}</span>
    <button onclick={() => movePosBy(1)} disabled={pos.index === pos.maxIndex}>›</button>
</div>

<style>
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