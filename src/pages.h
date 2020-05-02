const char WIFI_CONNECT_PAGE[] = R"=====(
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
        function reqListener() {

            var resp = this.responseText;
            if (resp == "0.0.0.0" ? alert("Wifi não CONECTADO") : alert("Wifi conectado com o IP: " + resp));

        }

        function updateWifi(formSub) {
            var xhr = new XMLHttpRequest();
            var ssid = document.getElementById("ssid").value;
            var pass = document.getElementById("pass").value;

            if (ssid != "" && pass != "") {

                xhr.addEventListener("load", reqListener);
                xhr.open("POST", "/config/wifi");
                xhr.send(new FormData(formSub));
            }
        }
    </script>
</head>
<form id="formWifi" style="text-align: center;" method="POST">
    <h1>Configuração do WIFI</h1>
    <b>SSID:</b><input name="ssid_wifi" id="ssid" placeholder="SSID" required><br />
    <b>PASS:</b><input name="pass_wifi" id="pass" type="password" placeholder="Password" required><br />
    <button onclick="updateWifi(form)">Conectar</button>
</form>
)=====";