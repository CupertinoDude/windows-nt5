<job id="Cluster Properties">

	<reference guid="{f2e606e0-2631-11d1-89f1-00a0c90d061e}" version="1.0" />

	<script language="VBScript">

		Option Explicit

		Dim oCluster
		Dim oGroup
		Dim oNode
		Dim vPending

		Set oCluster = CreateObject("MSCluster.Cluster")

		oCluster.Open ("galenb-clus")
		MsgBox oCluster.Name

		set oGroup = oCluster.ResourceGroups.Item( "Test" )
		set oNode = oCluster.Nodes.Item( "Galenb-C1" )

		vPending = oGroup.Online( 1000 )

	</script>
</job>
