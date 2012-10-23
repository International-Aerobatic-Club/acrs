<?php
/*  attendeeMap.php, acrs/admin, dlco, 10/20/2012
 *  View map of contest registrants by zip code
 */
set_include_path('./include'); 
require ("ui/validate.inc");
require ("data/validCtst.inc");
require_once ('post/setContest.inc');
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ('data/timecheck.inc');
require_once ("useful.inc");
require_once ("ui/siteLayout.inc");
require_once ("query/userQueries.inc");

function generate_marker_data($db_conn, $ctstID)
{
   $query = 'select a.postalCode, ' .
    ' e.name, e.hasTeamReg, b.teamAspirant ' .
    ' from registrant a, registration b, ctst_cat e, reg_type f' .
    ' where a.userID = f.userID ' .
    ' and f.ctstID = ' . $ctstID .
    " and f.compType = 'competitor'" .
    ' and b.regID = f.regID' .
    ' and e.catID = b.catID' .
    ' order by a.postalCode';
    //debug('generate_marker_data:'.$query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
     echo '<p class="error">' .
       notifyError(dbErrorText(),'generate_marker_data') . '</p>';
   }
   else
   {
     if (0 < dbCountResult($result))
     {
       echo 'var records = [';
       $first = true;
       while ($curRcd = dbFetchAssoc($result))
       {
         if (!$first) echo ",\n";
         $first = false;
         echo "{zip:'" . $curRcd['postalCode'] .  "',cat:'" . $curRcd['name'] . "'}";
       }
       echo "];\n";
     }
   }
}

$fail = dbConnect($db_conn);
$ctstID = $_SESSION['ctstID'];
startHead("Contest Participant Map");
if ($fail == '') {
?>
<meta name="viewport" content="initial-scale=1.0, user-scalable=no" />
<script type="text/javascript">
  function displayMap() {
    <?php generate_marker_data($db_conn, $ctstID) ?>
    var mapOptions = {
      center: new google.maps.LatLng(38.68,-96),
      zoom: 5,
      mapTypeId: google.maps.MapTypeId.ROADMAP
    };
    var map = new google.maps.Map(document.getElementById("map_canvas"), mapOptions);
    geocoder = new google.maps.Geocoder();
    last_position = '';
    function mark_record_list(ra) 
    {
      if (!ra.empty)
      {
        var cur = 0;
        record = ra[0];
        geocoder.geocode( { 'address': record.zip }, function(results, status) {
          if (status == google.maps.GeocoderStatus.OK) {
            while (cur < ra.length && ra[cur].zip == record.zip)
            {
              var marker = new google.maps.Marker({
                map: map,
                position: results[0].geometry.location,
                title: ra[cur].cat
              });
              cur += 1;
            }
          } 
          else
          {
            console.log("zip %s returned status %d", record.zip, status);
          }
          if (cur < ra.length)
          {
            setTimeout(function(){mark_record_list(ra.slice(cur));},250);
          }
        });
      }
    }
    mark_record_list(records);
  }

  function loadScript() {
    var script = document.createElement("script");
    script.type = "text/javascript";
    script.src="http://maps.googleapis.com/maps/api/js?key=AIzaSyAoGeFLs6fqDgfOGVOwThfBqrKouS9TQfQ&sensor=false&callback=displayMap";
    document.body.appendChild(script);
  }
  window.onload = loadScript;
</script>
<?php
}
startContent();
echo '<h1>Contest Participant Map</h1>';
echo '<a href="index.php">Back</a>';
echo '<div id="map_canvas"></div>';
endContent();
?>
