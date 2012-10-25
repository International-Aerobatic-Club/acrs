  function makeImage(letter,color)
  {
    return new google.maps.MarkerImage(
      "http://chart.apis.google.com/chart?chst=d_map_pin_letter&chld="+letter+"|"+color,
      new google.maps.Size(21,34),
      new google.maps.Point(0,0),
      new google.maps.Point(10,34));
  }
  function displayMap() {
    var img = [];
    img['u'] = makeImage('U','f55');
    img['a'] = makeImage('A','3f3');
    img['i'] = makeImage('I','faf');
    img['s'] = makeImage('S','aaf');
    img['p'] = makeImage('P','3ff');
    img['T'] = makeImage('T','ff3');
    var shadow = new google.maps.MarkerImage(
      "http://chart.apis.google.com/chart?chst=d_map_pin_shadow",
      new google.maps.Size(40, 37),
      new google.maps.Point(0, 0),
      new google.maps.Point(12, 35));
    var mapOptions = {
      center: new google.maps.LatLng(38.68,-96),
      zoom: 5,
      mapTypeId: google.maps.MapTypeId.ROADMAP
    };
    var map = new google.maps.Map(document.getElementById("map_canvas"), mapOptions);
    geocoder = new google.maps.Geocoder();
    interval = 350;
    timeout = interval;
    function mark_record_list(ra) 
    {
      if (!ra.empty)
      {
        var cur = 0;
        record = ra[0];
        geocoder.geocode( { 'address': record.zip }, function(results, status) {
          if (status == google.maps.GeocoderStatus.OK) {
            var markAt = results[0].geometry.location;
            while (cur < ra.length && ra[cur].zip == record.zip)
            {
              var letter;
              if (ra[cur].team == 'true') {
                letter = 'T';
              }
              else {
                letter = ra[cur].cat[0];
              }
              var marker = new google.maps.Marker({
                map: map,
                position: new google.maps.LatLng(markAt.Ya, markAt.Za),
                title: ra[cur].name,
                icon: img[letter],
                shadow: shadow
              });
              markAt.Ya += 0.1;
              markAt.Za += 0.1;
              cur += 1;
            }
            timeout = interval;
          } 
          else if (status == google.maps.GeocoderStatus.ZERO_RESULTS)
          {
            while (cur < ra.length && ra[cur].zip == record.zip)
            {
              cur += 1;
            }
          }
          else if (status == google.maps.GeocoderStatus.OVER_QUERY_LIMIT)
          {
            timeout += interval;
          }
          else
          {
            cur += 1;
          }
          if (cur < ra.length)
          {
            setTimeout(function(){mark_record_list(ra.slice(cur));},timeout);
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
