<script lang="ts">
    import { Direction, RelativePos } from "./types";

	type Props = {
		offsetX: number;
        offsetY: number;
        direction: Direction;
        relativepos: RelativePos;
        height: number;
		amount: number;
        pinLength?: number;
        pinWidth?: number;
        deadZone?: number;
	};

	let {
		offsetX,
		offsetY,
        direction,
        relativepos,
		height,
		amount,
        pinLength = 15,
        pinWidth = 8,
        deadZone = 10
	}: Props = $props();

    const spaceing = $derived((height - 2 * deadZone) / amount);

    //orientation correction
    const orientationCorrecctPinLenght = $derived.by(() => {
        if (relativepos === RelativePos.Left || relativepos === RelativePos.Right) return pinLength;
        return pinWidth;
    });

    const orientationCorrecctPinWidth = $derived.by(() => {
        if (relativepos === RelativePos.Left || relativepos === RelativePos.Right) return pinWidth;
        return pinLength;
    });

    //offset correction
    const correctedOffsetX = $derived.by(() => {
        if (relativepos === RelativePos.Left) return offsetX - pinLength;
        return offsetX;
    });

    const correctedOffsetY = $derived.by(() => {
        if (relativepos === RelativePos.Top) return offsetY - pinLength;
        return offsetY;
    });
    

    function calcPinPos(i: number, dir: Direction) {
        if (dir === direction) {
            return i * spaceing + deadZone + ((spaceing-pinWidth) / 2);
        }
        return 0;
    }
</script>


{#each Array(amount) as _, i}
		<rect
			x={correctedOffsetX + calcPinPos(i, Direction.Horizontal)}
			y={correctedOffsetY + calcPinPos(i, Direction.Vertical)}
			width={orientationCorrecctPinLenght}
			height={orientationCorrecctPinWidth}
			class="pin"
		/>
{/each}


<style>
    .pin {
		fill: rgb(104, 98, 91);
	}
</style>