const char WIFI_CONNECT_PAGE[] = R"=====(
<html>

<head>
    <meta charset="UTF-8">
    <title>Config WIFI</title>
    <style>
        button {
            background-color: #158e15b3;
            height: 35px;
            width: 15%;
            border-radius: 10px;
            margin-top: 1%;
        }
        
        h1 {
            font-family: Arial;
            padding-top: 10%;
            font-size: 140%;
        }
        
        input {
            border-radius: 3px;
            padding-left: 1%;
            margin-left: 2%;
            margin-top: 1%;
            width: 15%;
            background-color: #e2e2e2;
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
<div id="formWifi" style="text-align: center;">
    <h1>Configuração do WIFI</h1>
    <b>SSID:</b><input name="ssid_wifi" id="ssid" placeholder="SSID" required><br />
    <b>PASS:</b><input name="pass_wifi" id="pass" type="password" placeholder="Password" required><br />
    <button onclick="updateWifi()">Conectar</button><br /><br />

    <div id="resposta_wifi"></div>
</div>

</html>
)=====";

const char WIFI_STATUS_PAGE[] = R"=====(
    
    WEDFWEFW

)=====";