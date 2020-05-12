const char WIFI_CONNECT_PAGE[] = R"=====(
<html>

<head>
    <meta charset="UTF-8">
    <title>Config WIFI</title>
    <style>
        *{
            font-family: Arial, Helvetica, sans-serif;
            margin:0;
            padding:0;
        }

        button {
            background-color: #158e15b3;
            height: 35px;
            width: 15%;
            border-radius: 10px;
            margin-top: 1%;
        }
        
        input {
            border-radius: 3px;
            padding-left: 1%;
            margin-left: 2%;
            margin-top: 1%;
            width: 15%;
            background-color: #e2e2e2;
        }
        section {
            width: 100%;
            height: 100%;
        }
        .container{
            width: inherit;
            height: inherit;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        .wifi-card{
            /* width: 25vw;
            height: 45vh; */
            width: 500px;
            height: 350px;
            border-radius: 25px;
            /* border: 1px solid #444; */
            padding: 20px 20px;
            display: flex;
            flex-direction: column;
            box-shadow: 0 1px 2px rgba(0,0,0,0.07), 0 2px 4px rgba(0,0,0,0.07), 0 4px 8px rgba(0,0,0,0.07), 0 8px 16px rgba(0,0,0,0.07), 0 16px 32px rgba(0,0,0,0.07), 0 32px 64px rgba(0,0,0,0.07);
        }
        .wifi-card .card-title{
            font-size: 2.5em;
            text-align: center;
            flex:0.25;
        }
        .wifi-card .card-subtitle{
            font-size: 1.2em;
            text-align: center;
            margin-top: 20px;
            /* flex:0.25; */
            flex:1;
        }
        .wifi-card .wifi-form{
            /* margin-top: 50px; */
            padding: 20px;
            border-radius: 12.5px;
            background-color: #f5f5f5;
            /* box-shadow: 0 1px 2px rgba(0,0,0,0.07), 0 2px 4px rgba(0,0,0,0.07), 0 4px 8px rgba(0,0,0,0.07), 0 8px 16px rgba(0,0,0,0.07), 0 16px 32px rgba(0,0,0,0.07), 0 32px 64px rgba(0,0,0,0.07); */
        }
        .wifi-card .input-label{
            width: 100px;
            font-size: 1.2em;
        }
        .wifi-card .input{
            display: flex;
            align-items: center;
            justify-content: space-between;
        }

        .wifi-card .input:nth-child(2){
            margin-top:10px;
        }

        .wifi-card .input input{
            width: 100%;
            background-color: white;
            height: 40px;
            /* border: 1px solid #333; */
            font-size: 1.2em;
            border-radius: 4px;
            box-shadow: none;
            border: 1px solid #fbfbfb;
            padding: 10px;
        }

        .wifi-card .submit-btn{
            border-radius: 8px;
            font-size: 1.3em;
            line-height: 1.3em;
            font-weight: bold;
            text-transform: uppercase;
            width: auto;
            height: auto;
            display: block;
            padding: 10px 20px;
            margin: 0 auto;
            margin-top: 20px;
            color: #fff;
            border:0
        }
    </style>
    <script>
        function wait(ms) {
            var start = new Date().getTime();
            var end = start;
            while (end < start + ms) {
                end = new Date().getTime();
            }
        }

        function getStateWifi() {

            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/status/wifi/state", false);
            xhr.send();

            if (xhr.responseText == "desconectado") {
                wait(1000);
                getStateWifi();
            } else if (xhr.responseText == "conectando") {
                document.getElementById("resposta_wifi").innerHTML = "Conectando ao wifi...";
                wait(1000);
                getStateWifi();
            } else if (xhr.responseText == "timeout") {
                document.getElementById("resposta_wifi").innerHTML = "Erro ao conectar ao wifi...";
                document.getElementById("ssid").innerHTML = "";
                document.getElementById("pass").innerHTML = "";
            } else {
                document.getElementById("resposta_wifi").innerHTML = "Conectado com o IP: " + xhr.responseText;
                document.getElementById("ssid").innerHTML = "";
                document.getElementById("pass").innerHTML = "";
            }
        }

        function updateWifi() {
            var xhr = new XMLHttpRequest();
            var formData = new FormData();
            var ssid = document.getElementById("ssid").value;
            var pass = document.getElementById("pass").value;

            formData.append("ssid_wifi", ssid);
            formData.append("pass_wifi", pass);

            if (ssid == "" || pass == "") {

                alert("Existem campos vazios !!");

            } else {

                xhr.open("POST", "/config/wifi");
                xhr.send(formData);

                document.getElementById("resposta_wifi").innerHTML = "Conectando... Aguarde !!";
                getStateWifi();
            }
        }
    </script>
</head>
<body>
    <section>
        <div class="container">
            <div class="wifi-card">
                <h1 class="card-title">Configuração do Wi-Fi</h1>
                <p class="card-subtitle">Preencha os dados abaixo para se conectar ao dispositivo.</p>
                <div class="wifi-form">
                    <div class="input">
                        <p class="input-label">SSID: </p>
                        <input name="ssid_wifi" id="ssid" placeholder="SSID" required>        
                    </div>
                    <div class="input">
                        <p class="input-label">Senha: </p>
                        <input name="pass_wifi" id="pass" type="password" placeholder="Senha" required>     
                    </div>
                    <button class="submit-btn" onclick="updateWifi()">Conectar</button>
                </div>
            </div>
        </div>
    </section>
</body>
<!-- <div id="formWifi" style="text-align: center;">
    <h1>Configuração do WIFI</h1>
    <b>SSID:</b><input name="ssid_wifi" id="ssid" placeholder="SSID" required><br />
    <b>PASS:</b><input name="pass_wifi" id="pass" type="password" placeholder="Password" required><br />
    <button onclick="updateWifi()">Conectar</button><br /><br />

    <div id="resposta_wifi"></div>
</div> -->

</html>
)=====";

const char WIFI_STATUS_PAGE[] = R"=====(
    
    WEDFWEFW

)=====";