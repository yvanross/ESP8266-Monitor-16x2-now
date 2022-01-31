const char HOME_page[] PROGMEM = R"=====(
<HTML>
  <HEAD>
      <TITLE>Erabliere graphique</TITLE>
      <meta http-equiv="refresh" content="$$refresh_rate$$">
      <link rel="stylesheet" href="./style.css" type="text/css" />
      <script src="./microajax.min.js"</script>
  </HEAD>
<BODY>
  
  <form action="/" style="display:inline;">
  <button>Page principale</button>
  </form> 
  
  <form action="/config" style="display:inline;">
  <button>Configuration</button>
  </form> 
    
   <div id="container" style="width:75%; height:75%;">
      <canvas id="line-chart"></canvas>
  </div>

info seuil:
    <div id="mydynamicdata>Here comes the Dynamic Data in </div>   <!-- added a DIV, where the dynamic data goes to --> 
   <script>                
      setValues("/filldynamicdata");  //-- this function calls the function on the ESP      
   </script>

    <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.5.0/Chart.min.js"></script>
       <script>
      
       new Chart(document.getElementById("line-chart"), {
          type: 'line',
          data: {
            labels: $$labels$$,
            datasets: [{ 
                data: $$levels$$,
                label: '$$label1$$',
                borderColor: "#3e95cd",
                fill: false
              },
            ]
          },
          options: {
            title: {
              display: true,
              text: 'Niveau sonore'
            },
            animation: {
                duration: 0
            },
            scales: {
             yAxes: [{
              display: true,
              position: 'right',
              ticks: {
               beginAtZero: true,
               max: 255,
               min: 0,
               stepSize: 5
              }
             }, {
              display: true,
              position: 'left',
              ticks: {
               beginAtZero: true,
               max: 255,
               min: 0,
               stepSize: 5
              }
             }]
           },
          }
        });
       </script>

  
</BODY>
</HTML>
)=====";
