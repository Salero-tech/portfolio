<script>
    import { pos, nodes } from './position.svelte.ts';
</script>


{#each nodes as n, i}
    <div
    class="node"
    class:active={i === pos.index}
    style="left:{n.x}px; top:{n.y}px; width:{n.w}px; height:{n.h}px;"
    >
        <h2>{n.label}</h2>
        <p>Node {i + 1} of {nodes.length}</p>
    </div>
{/each}

<style>
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
</style>