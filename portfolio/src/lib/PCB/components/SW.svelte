<script lang="ts">
    import Pins from "./parts/Pins.svelte";
    import { RelativePos, Direction } from "./parts/types";

    type Props = {
        x?: number;
        y?: number;
        width?: number;
        height?: number;
        label?: string;
    };

    let { x = 0, y = 0, width = 200, height = 120, label = 'SW' }: Props = $props();
</script>

<g transform={`translate(${x} ${y})`}>
    <!-- silkscreen -->
    <rect x="-8" y="-8" width={width + 16} height={height + 16} class="silk" rx="6" />

    <!-- body -->
    <rect width={width} height={height} rx="6" class="body" />

    <!-- pads (4) -->
    <Pins offsetX={0} offsetY={height} direction={Direction.Horizontal} relativepos={RelativePos.Bottom} height={width} amount={8} pinLength={20} pinWidth={10} deadZone={5} />
    <Pins offsetX={0} offsetY={0} direction={Direction.Horizontal} relativepos={RelativePos.Top} height={width} amount={8} pinLength={20} pinWidth={10} deadZone={5} />

    <text x={width/2} y={height/2} class="label">{label}</text>
</g>

<style>
    .body { fill: #212121; stroke: #333; stroke-width: 2 }
    .silk { fill: none; stroke: white; stroke-width: 1.5 }
    .pad { fill: #6b6158 }
    .label { fill: white; text-anchor: middle; dominant-baseline: middle; font-size: 14px }
</style>
