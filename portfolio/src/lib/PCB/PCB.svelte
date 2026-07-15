<script lang="ts">
	import QFP from '$lib/PCB/components/QFP.svelte';
	import SW from '$lib/PCB/components/SW.svelte';
	import TO252 from '$lib/PCB/components/TO252.svelte';
	import SOT23 from '$lib/PCB/components/SOT23.svelte';
</script>

<div class="pcb">

	<svg
		viewBox="0 0 2000 1200"
	>
		<!-- board -->
		<rect
			width="2000"
			height="1200"
			rx="30"
			class="pcb"
		/>

		<!-- copper -->

		<!-- silkscreen -->

		<QFP
			x={600}
			y={300}
			width={300}
			pins={64}
			label="STM32"
		/>

		<QFP
			x={200}
			y={300}
			width={300}
			pins={64}
			label="STM32"
		/>

		<!-- additional footprints -->
		<SW x={1200} y={200} width={160} height={90} label="SW1" />
		<TO252 x={1400} y={800} label="U1" />
		<SOT23 x={300} y={900} label="Q1" />
	</svg>


	<div class="components">
	</div>

</div>


<style>
	.pcb {
	fill: #064d24;
}

</style>