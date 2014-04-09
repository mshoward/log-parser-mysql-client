<div align =center>
<?php
  @ $db = mysqli_connect('honeypot1.db.7456864.hostedresource.com ', 'honeypot1', 'Honeypot1#', 'honeypot1');
//Check connection

  if (mysqli_connect_errno()) 
  {
   echo 'Connection to database failed:'.mysqli_connect_error();
   exit();
  }
  $query = "select * from Information"; 
  $result = $db->query($query);
  
 echo "<table border='2'>
 		<table width=1200px;>
 			
  <tr>
  			<th bgcolor=\"#3399ff\">ID Number</th>
  			<th bgcolor=\"#3399ff\">Date/Time</th>
  			<th bgcolor=\"#3399ff\">Packet Type</th>
  			<th bgcolor=\"#3399ff\">Source IP</th>
  			<th bgcolor=\"#3399ff\">Source Socket</th>
  			<th bgcolor=\"#3399ff\">Target IP</th>
  			<th bgcolor=\"#3399ff\">Target Socket</th>
  			<th bgcolor=\"#3399ff\">Version</th>
  </tr>";

  	while ($row = mysqli_fetch_array($result))
  	{
   	echo "<tr bgcolor=\"#ffcc99\">";
  	echo "<td>" . $row['ID'] . "</td>";
  	echo "<td>" . $row['DateTime'] . "</td>";
  	echo "<td>" . $row['packetType'] . "</td>";
  	echo "<td>" . $row['sourceIP'] . "</td>";
  	echo "<td>" . $row['sourceSocket'] . "</td>";
  	echo "<td>" . $row['targetIP'] . "</td>";
  	echo "<td>" . $row['targetSocket'] . "</td>";
  	echo "<td>" . $row['Version'] . "</td>";
  	echo "</tr>";
  	}
  	echo "</table>";














$searchtype = mysql_real_escape_string($searchtype);
$searchterm = mysql_real_escape_string($searchterm);


































