<script lang="ts">
	import QFP from '$lib/PCB/components/QFP.svelte';
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
	</svg>


	<div class="components">
	</div>

</div>


<style>
	.pcb {
	fill: #064d24;
}

</style>