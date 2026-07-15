<script lang="ts">
    import Pins from './parts/Pins.svelte';
    import { RelativePos, Direction } from './parts/types';

    type Props = {
        x?: number;
        y?: number;
        width?: number;
        height?: number;

        label?: string;
    };

    let { x = 0, y = 0, width = 140, height = 140, label = 'TO-252' }: Props = $props();

    const pinLen = 30;
    const pinW = 15;
    const bigPin = 80;
</script>

<g transform={`translate(${x} ${y})`}>
    <!-- outline / silkscreen -->
    <rect x="-10" y="-10" width={width + 20} height={height + 20} class="silk" rx="6" />
    <!-- body -->
    <rect width={width} height={height} class="body" rx="6" />

    <!-- pins along bottom edge (3) -->
    <Pins offsetX={0} offsetY={height} direction={Direction.Horizontal} relativepos={RelativePos.Bottom} height={width} amount={3} pinLength={pinLen} pinWidth={pinW} deadZone={10} />
    <!-- pin along top edge (1) -->
    <Pins offsetX={0} offsetY={0} direction={Direction.Horizontal} relativepos={RelativePos.Top} height={width} amount={1} pinLength={pinLen} pinWidth={bigPin} deadZone={10} />

    <text x={width/2} y={height/2} class="label">{label}</text>
</g>

<style>
    .body { fill: #151515; stroke: #222; stroke-width: 2 }
    .silk { fill: none; stroke: white; stroke-width: 1.5 }
    .label { fill: white; text-anchor: middle; dominant-baseline: middle; font-size: 14px }
</style>
