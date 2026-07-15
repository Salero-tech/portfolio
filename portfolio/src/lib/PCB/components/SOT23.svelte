<script lang="ts">
    import Pins from "./parts/Pins.svelte";
    import { RelativePos, Direction } from "./parts/types";

    type Props = {
        x: number;
        y: number;
        width?: number;
        height?: number;
        label?: string;
    };

    let { x, y, width = 100, height = 40, label = 'SOT-23' }: Props = $props();

    const padW = 20;
    const padH = 10;
</script>

<g transform={`translate(${x} ${y})`}>
    <!-- silkscreen -->
    <rect x="-8" y="-8" width={width + 16} height={height + 16} class="silk" rx="6" />
    <!-- body -->
    <rect width={width} height={height} rx="6" class="body" />

    <!-- left two pins -->
     <Pins offsetX={0} offsetY={height} direction={Direction.Horizontal} relativepos={RelativePos.Bottom} height={width} amount={2} pinLength={padW} pinWidth={padH} deadZone={0} />
     <Pins offsetX={0} offsetY={0} direction={Direction.Horizontal} relativepos={RelativePos.Top} height={width} amount={1} pinLength={padW} pinWidth={padH} deadZone={0} />

    <text x={width/2} y={height/2} class="label">{label}</text>
</g>

<style>
    .body { fill: #151515; stroke: #222; stroke-width: 1.5 }
    .silk { fill: none; stroke: white; stroke-width: 1 }
    .label { fill: white; text-anchor: middle; dominant-baseline: middle; font-size: 12px }
</style>
