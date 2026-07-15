<script lang="ts">
    import Pins from "./parts/Pins.svelte";
    import { Direction, RelativePos } from "./parts/types";

	type Props = {
		x: number;
		y: number;
		width?: number;
		pins?: number;
		label?: string;
	};

	let {
		x,
		y,
		width = 120,
		pins = 8,
		label = "MCU"
	}: Props = $props();

	const pinLength = 18;
	const pinWidth = 8;
	const deadZone = 10;

	const pinsPerSide = $derived(pins / 4);
</script>

<g transform={`translate(${x} ${y})`}>

	<!-- silkscreen outline -->
	<rect
		x="-10"
		y="-10"
		width={width + 20}
		height={width + 20}
		class="silk"
	/>

	<!-- IC body -->
	<rect
		width={width}
		height={width}
		rx="8"
		class="body"
	/>

	<!-- orientation mark -->
	<circle
		cx="20"
		cy="20"
		r="5"
		class="mark"
	/>

	<!-- top pins -->
	<Pins offsetX={0} offsetY={0} direction={Direction.Horizontal} relativepos={RelativePos.Top} height={width} amount={pinsPerSide} pinLength={pinLength} pinWidth={pinWidth} deadZone={deadZone} />
	<!-- bottom pins -->
	<Pins offsetX={0} offsetY={width} direction={Direction.Horizontal} relativepos={RelativePos.Bottom} height={width} amount={pinsPerSide} pinLength={pinLength} pinWidth={pinWidth} deadZone={deadZone} />
	<!-- left pins -->
	<Pins offsetX={0} offsetY={0} direction={Direction.Vertical} relativepos={RelativePos.Left} height={width} amount={pinsPerSide} pinLength={pinLength} pinWidth={pinWidth} deadZone={deadZone} />
	<!-- right pins -->
	<Pins offsetX={width} offsetY={0} direction={Direction.Vertical} relativepos={RelativePos.Right} height={width} amount={pinsPerSide} pinLength={pinLength} pinWidth={pinWidth} deadZone={deadZone} />

	<text
		x={width / 2}
		y={width / 2}
		class="label"
	>
		{label}
	</text>

</g>


<style>
	.body {
		fill: #151515;
		stroke: #333;
		stroke-width: 2;
	}

	.silk {
		fill: none;
		stroke: white;
		stroke-width: 2;
	}

	.mark {
		fill: white;
	}

	.label {
		fill: white;
		text-anchor: middle;
		dominant-baseline: middle;
		font-family: monospace;
		font-size: 16px;
	}
</style>