/*****************************************************************/
// STORAGE
/*****************************************************************/

var hostname = window.location.hostname;
var agents = {}; // {CLASS : { ID : GeoJSONLayer }}
var layers = {}; // {CLASSNAME : FeatureLayerGroup}
var switcher; // Layer Switcher control


/*****************************************************************/
// MAP
/*****************************************************************/


// Create map
var map = L.map('map').setView([39.83, -3.4], 7);
// Add hash to location URL
new L.Hash(map);
// Add scale
L.control.scale().addTo(map);


/*****************************************************************/
// ENVIRONMENT DATE TIME
/*****************************************************************/


var dateDiv, dateInverval;
var datePanel = L.control({position: 'topright'});
datePanel.onAdd = function (map) {
  dateDiv = L.DomUtil.create('div', 'date styled');
  return dateDiv;
}
map.addControl(datePanel);


/*****************************************************************/
// LAYERS
/*****************************************************************/


var OSMLayer = L.tileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
maxZoom: 22,
maxNativeZoom: 19
}).addTo(map);

var tonerLayer = L.tileLayer('http://{s}.tile.stamen.com/toner/{z}/{x}/{y}.png', {
maxZoom: 22,
maxNativeZoom: 18
});

var satLayer = L.tileLayer('http://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}.jpg', {
maxZoom: 22,
maxNativeZoom: 18
});

// Create layer switcher
switcher = createLayerSwitcher(false, layers);


/*****************************************************************/
// GEOMETRIES
/*****************************************************************/


// Style
function getStyle(feature) {
  return {
    fillColor: feature.properties.color || 'gray',
    weight: feature.properties.weight || 6,
    opacity: feature.properties.border_opacity || 0.8,
    color: feature.properties.border_color || 'black',
    fillOpacity: feature.properties.fill_opacity || 0.5,
    dashArray: feature.properties.dash_array || 1
  };
}

// Highlight
function onEachHighlightFeature(feature, layer) {
  layer.on({
    mouseover: function(e) {
      var popupContent = '<h2>'+feature.properties.name+'</h2>\
      <table>';
      for (var i in feature.properties){
	popupContent += '<tr><td>'+i+'</td><td>'+feature.properties[i]+'</td></tr>';
      }
      popupContent += '</table>';
      layer.bindPopup(popupContent);
    },
    mouseout: function(e) {
    },
    click: function(e) {
    }
  });
}

// Point geojson, set marker icon and if draggable
function pointToLayer(feature, latlng){
  var draggable = feature.properties.draggable || false;
  var title = feature.properties.name;
  var marker;
  if (feature.properties.icon_url){
    var icon = L.icon({
	      iconUrl: feature.properties.icon_url || '',
	      iconSize: feature.properties.icon_size || [32, 32],
	      iconAnchor: feature.properties.icon_anchor || [16, 32],
	      popupAnchor: feature.properties.popup_anchor || [0, -24]
    });
    marker = L.marker(latlng, {icon: icon, draggable: draggable, title: title});
  } else {
    marker = L.marker(latlng, {draggable: draggable, title: title});
  }
  if (draggable){
    marker.on('dragend', function (e) {
      var coords = e.target.getLatLng();
      updateOne(feature.properties.class, feature.properties.id, {x : coords.lng, y : coords.lat});
      });
  }
  return marker;
}


/*****************************************************************/
// SOCKET
/*****************************************************************/


// Create socket
var hostname = window.location.hostname;
var socket = new WebSocket("ws://" + hostname + ":3001");

// Bind events
socket.onopen = function (event) {
}
socket.onmessage = function (event) {
  parseMessage(JSON.parse(event.data));
}

function parseMessage(json_message){
  
  var json_data = json_message.data,
  operation = json_message.operation;
  
  // data is an array, execute the same operation for all the elements inside it
  if( Object.prototype.toString.call( json_data ) === '[object Array]' ) {
    for(var i in json_data){
      parseAgent(json_data[i], operation);
    }
  // Else, data will only contain one element
  } else {
      parseAgent(json_data, operation);
  }
}

function parseAgent(json_message, operation){
      switch (operation){

	case 0: // Create entity
	case 1: // Update entity
	  // If it is the first agent we receive from a class, create the agents storage and layer switcher
	  if (!agents[json_message.class]){
		agents[json_message.class] = {};
		layers[json_message.class] = L.featureGroup().addTo(map);
		switcher = createLayerSwitcher(switcher, layers);
	  }
	  // Delete from layers and memory
	  if (agents[json_message.class][json_message.id]){
		  layers[json_message.class].removeLayer(agents[json_message.class][json_message.id]);
		  agents[json_message.class][json_message.id] = 0;
	  }
	  // Create geojson layer and add it to layers
	  agents[json_message.class][json_message.id] = L.geoJson( json_message.geojson, {style: getStyle, pointToLayer: pointToLayer,  onEachFeature: onEachHighlightFeature} );

	  if(json_message.map_follow){
	    map.fitBounds(agents[json_message.class][json_message.id].getBounds(), {maxZoom : 17});
	  }
	  layers[json_message.class].addLayer(agents[json_message.class][json_message.id]);
	  break;
	  
	case 2: // Delete entity
	  // Delete from agentsLayer and memory
	  if (agents[json_message.class][json_message.id]){
		  layers[json_message.class].removeLayer(agents[json_message.class][json_message.id]);
		  agents[json_message.class][json_message.id] = 0;
	  }
	  break;
	default:
	  new Error("Unknown operation type" , operation);
      }
}

function createLayerSwitcher(old, layers){
  if (old){
      old.removeFrom(map);
  }
  return L.control.layers({OpenStreetMap : OSMLayer, Toner : tonerLayer, Satelite : satLayer}, layers).addTo(map);
}

/*****************************************************************/
// PREMADE FUNCTIONS
/*****************************************************************/


function getEnvironment() {
  $.get("http://" + hostname + ":3000/environment", function(response){
      parseMessage(JSON.parse(response));
      map.fitBounds(agentsLayer.getBounds());
  });
};

function getAll(class_name) {
  $.get("http://" + hostname + ":3000/" + class_name, function(response){
      parseMessage(JSON.parse(response));
  });
};

function getOne(class_name, id) {
  $.get("http://" + hostname + ":3000/" + class_name + "/" + id, function(response){
      parseMessage(JSON.parse(response));
  });
};

function updateOne(class_name, id, parameters) {
  $.ajax({
   url: "http://" + hostname + ":3000/" + class_name + "/" + id,
   type: 'PUT',
   data: parameters,
   success: function(response) {
     parseMessage(JSON.parse(response));
   }
  });
};

function getAllAsHeatMap(class_name, size) {
  $.get("http://" + hostname + ":3000/" + class_name, function(response){
      var features = JSON.parse(response).data;
      if (features.length){
	var heatmap = new L.TileLayer.WebGLHeatMap({size: size, autoresize: true, alphaRange: 0.5, opacity: 0.5});
	for (var i = 0; i < features.length; i++) {
	    heatmap.addDataPoint(features[i].y, features[i].x, 1);
	}
	console.log(heatmap);
      map.addLayer(heatmap);
      }
  });
};

function runOne(class_name, id) {
  $.post("http://" + hostname + ":3000/" + class_name + "/" + id + "/run");
}

function runAll(class_name) {
  $.post("http://" + hostname + ":3000/" + class_name + "/run");
};

function watchTime(seconds){
  $.get("http://" + hostname + ":3000/environment/time", function(data){
    dateDiv.innerHTML = new Date(JSON.parse(data).time);
    setTimeout(function(){watchTime(seconds)}, 1000 * (seconds || 1));
  });
}

