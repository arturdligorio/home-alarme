const char WIFI_CONNECT_PAGE[] = R"=====(
<html>

<head>
    <meta charset="UTF-8">
    <title>Config WIFI</title>
    <style>
        * {
            font-family: Arial, Helvetica, sans-serif;
            margin: 0;
            padding: 0;
        }
        
        button {
            height: 35px;
            width: 15%;
            border-radius: 10px;
            margin-top: 1%;
            cursor: pointer;
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
        
        .container {
            width: inherit;
            height: inherit;
            display: flex;
            justify-content: center;
            align-items: center;
            flex-direction: column;
        }
        
        .wifi-card {
            /* width: 25vw;
            height: 45vh; */
            position: relative;
            width: 500px;
            height: 350px;
            border-radius: 25px;
            /* border: 1px solid #444; */
            padding: 20px 20px;
            display: flex;
            flex-direction: column;
            box-shadow: 0 1px 2px rgba(0, 0, 0, 0.07), 0 2px 4px rgba(0, 0, 0, 0.07), 0 4px 8px rgba(0, 0, 0, 0.07), 0 8px 16px rgba(0, 0, 0, 0.07), 0 16px 32px rgba(0, 0, 0, 0.07), 0 32px 64px rgba(0, 0, 0, 0.07);
        }
        
        .wifi-card .card-title {
            font-size: 2.5em;
            text-align: center;
            flex: 0.25;
        }
        
        .wifi-card .card-subtitle {
            font-size: 1.2em;
            text-align: center;
            margin-top: 20px;
            /* flex:0.25; */
            flex: 1;
        }
        
        .wifi-card .wifi-form {
            /* margin-top: 50px; */
            padding: 20px;
            border-radius: 12.5px;
            background-color: #f5f5f5;
            text-align: center;
            /* box-shadow: 0 1px 2px rgba(0,0,0,0.07), 0 2px 4px rgba(0,0,0,0.07), 0 4px 8px rgba(0,0,0,0.07), 0 8px 16px rgba(0,0,0,0.07), 0 16px 32px rgba(0,0,0,0.07), 0 32px 64px rgba(0,0,0,0.07); */
        }
        
        .wifi-card .input-label {
            width: 100px;
            font-size: 1.2em;
        }
        
        .wifi-card .input {
            display: flex;
            align-items: center;
            justify-content: space-between;
        }
        
        .wifi-card .input:nth-child(2) {
            margin-top: 10px;
        }
        
        .wifi-card .input input {
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
        
        .wifi-card .submit-btn {
            background-color: #158e15b3;
            border-radius: 8px;
            font-size: 1.3em;
            line-height: 1.3em;
            font-weight: bold;
            width: 150px;
            height: auto;
            padding: 10px 20px;
            box-shadow: 0px 1px 5px 3px green;
            margin: 0 auto;
            margin-top: 20px;
            margin-left: 13px;
            color: #fff;
            border: 0;
        }
        
        .notification-wrapper {
            display: none;
            width: 500px;
            align-items: center;
            padding: 20px;
            border-radius: 25px;
            box-shadow: 0 1px 2px rgba(0, 0, 0, 0.07), 0 2px 4px rgba(0, 0, 0, 0.07), 0 4px 8px rgba(0, 0, 0, 0.07), 0 8px 16px rgba(0, 0, 0, 0.07), 0 16px 32px rgba(0, 0, 0, 0.07), 0 32px 64px rgba(0, 0, 0, 0.07);
            margin-bottom: 30px;
        }
        
        .notification-wrapper .notification-icon {
            width: 60px;
            margin-right: 25px;
        }
        
        .notification-wrapper .notification-icon svg {
            width: 60px;
        }
        
        .notification-wrapper .notification-info .notification-title {
            margin-bottom: 15px;
            font-size: 1.7em;
        }
        
        .show {
            display: flex;
        }
        
        .wrapper-wifi-answer {
            display: flex;
            justify-content: center;
            align-items: center;
            margin-top: 40px;
            padding: 15px 15px;
            text-align: center;
            font-weight: 600;
        }
        
        .go-home-btn {
            background-color: #f62a2a;
            border-radius: 8px;
            font-size: 1.3em;
            line-height: 1.3em;
            font-weight: bold;
            width: 150px;
            height: auto;
            padding: 10px 20px;
            box-shadow: 0px 1px 5px 3px #b65454;
            margin: 0 auto;
            margin-top: 20px;
            margin-left: 13px;
            color: #fff;
            border: 0;
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

        function show(notification, show) {
            // Notification = 'success' || 'error'
            const wrapper = document.getElementById(`${notification}-notification`);
            if (wrapper) {
                if (show) return wrapper.classList.add('show');
                wrapper.classList.remove('show');
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
                document.getElementById("ssid").value = "";
                document.getElementById("pass").value = "";
                show('error', true);
            } else {
                document.getElementById("resposta_wifi").innerHTML = "Conectado com o IP: " + xhr.responseText;
                document.getElementById("ssid").value = "";
                document.getElementById("pass").value = "";
                show('success', true);
            }
        }

        function updateWifi() {

            show('success', false);
            document.getElementById("resposta_wifi").innerHTML = "";

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
            <div class="notification-wrapper" id="error-notification">
                <div class="notification-icon">
                    <?xml version="1.0" encoding="UTF-8"?>
                    <svg enable-background="new 0 0 512 512" version="1.1" viewBox="0 0 512 512" xml:space="preserve" xmlns="http://www.w3.org/2000/svg">
                        <ellipse cx="256" cy="256" rx="256" ry="255.83" fill="#E04F5F" />
                        <g transform="matrix(-.7071 .7071 -.7071 -.7071 77.26 32)" fill="#fff">
                            <rect x="3.98" y="-427.62" width="55.992" height="285.67" />
                            <rect x="-110.83" y="-312.82" width="285.67" height="55.992" />
                        </g>
                    </svg>
                </div>
                <div class="notification-info">
                    <h1 class="notification-title">Não foi possível conectar</h1>
                    <p class="notification-subtitle">Ocorreu uma falha ao tentar conectar ao dispositivo. Por favor, tente novamente.</p>
                </div>
            </div>
            <div class="notification-wrapper" id="success-notification">
                <div class="notification-icon">
                    <?xml version="1.0" encoding="UTF-8"?>
                    <svg enable-background="new 0 0 512 512" version="1.1" viewBox="0 0 512 512" xml:space="preserve" xmlns="http://www.w3.org/2000/svg">
                        <ellipse cx="256" cy="256" rx="256" ry="255.83" fill="#32BEA6" />
                        <polygon
                            points="235.47 392.08 114.43 297.78 148.85 253.62 223.18 311.52 345.85 134.5 391.88 166.39"
                            fill="#fff" />
                    </svg>

                </div>
                <div class="notification-info">
                    <h1 class="notification-title">Conectado com sucesso!</h1>
                    <p class="notification-subtitle">A tentativa de conexão ao Wi-Fi foi efetuada com sucesso! Prossiga para o próximo passo!</p>
                </div>
            </div>
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
                    <button class="go-home-btn" onclick="window.location.href = window.location.origin + '/home'">Home</button>
                </div>
            </div>
            <div class="wrapper-wifi-answer">
                <div id="resposta_wifi"></div>
            </div>
        </div>
    </section>
</body>

</html>
)=====";

const char MOVIMENTO_MONITORING_PAGE[] = R"=====(    
<html>

<head>
    <meta charset="UTF-8">
    <title>MONITOR</title>
    <style>
        * {
            font-family: Arial, Helvetica, sans-serif;
            margin: 0;
            padding: 0;
        }
        
        body {
            background-color: snow;
            font-family: 'Courier New', Courier, monospace;
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
        
        .container-all {
            display: flex;
            flex-flow: row wrap;
            justify-content: center;
            align-items: center;
            padding: 50px 50px;
        }
        
        .flex {
            display: flex;
            flex-flow: row wrap;
            justify-content: center;
            align-items: center;
        }
        
        .header {
            flex: 0 0 100%;
            font-weight: 600;
            font-size: 25px;
            font-style: italic;
        }
        
        .sending-request {
            flex: 0 0 100%;
            font-weight: 500;
            font-size: 18px;
            padding: 50px 50px;
        }
        
        .event-list {
            list-style: none;
        }
        
        .events {
            align-items: flex-start;
            height: 60vh;
            overflow-y: auto;
        }
        
        .event-list li {
            background-color: black;
            padding: 10px 10px;
            margin-bottom: 20px;
            color: #fff;
            font-size: 15px;
            border-radius: 5px;
            width: 400px;
            height: 30px;
            animation: opacityTransition .4s;
            position: relative;
            transition: all .3s;
            display: flex;
            flex-flow: row wrap;
            align-content: center;
            box-shadow: 2px 1px 7px 2px gray;
            font-weight: 600;
        }
        
        @keyframes opacityTransition {
            from {
                opacity: 0;
            }
            ;
            to {
                opacity: 1;
            }
            ;
        }
        
        .navbar {
            margin-top: 30px;
        }
        
        .button-home {
            background-color: #158e15b3;
            color: #fff;
            width: 150px;
            text-align: center;
            padding: 6px 6px;
            border-radius: 6px;
            font-weight: 600;
            cursor: pointer;
            transition: all .6s;
            box-shadow: 0px 1px 5px 3px green;
        }
        
        .wrapper {
            height: 160px;
            background-color: #fff;
            width: 450px;
            padding: 30px 30px;
            padding-top: 60px;
            box-shadow: 0px 1px 6px 3px gray;
            border-radius: 13px;
        }
        
        .button-home:hover {
            transform: scale(1.05);
            transition: .6s;
        }
        
        .transition {
            animation: opacityTransition 1.5s;
        }
        
        .scroll1::-webkit-scrollbar {
            width: 5px;
        }
        
        .scroll1::-webkit-scrollbar-thumb {
            background: #666;
            border-radius: 20px;
        }
        
        .scroll1::-webkit-scrollbar-track {
            background: #ddd;
            border-radius: 10px;
        }
    </style>
</head>

<body>
    <section>
        <div class="container-all">
            <div class="wrapper">
                <div class="header flex">
                    <h3>Monitor de Eventos</h3>
                </div>
                <div class="navbar flex">
                    <div class="button-home" id="go-home">
                        Home
                    </div>
                </div>
            </div>
            <div class="sending-request flex" id="sending-request">
                <p>Coletando informações de eventos ...</p>
            </div>
            <div class="events scroll1 flex">
                <ul class="event-list" id="events-list">
                </ul>
            </div>
        </div>
    </section>
</body>
<script>
    function addListenerGoHome() {
        const button = document.getElementById('go-home');
        button.addEventListener('click', () => {
            window.location = '/home'
        })
    };

    function formatDate(d) {

        var day = new String(d.getDate());
        var month = new String(d.getMonth() + 1);
        var hours = new String(d.getHours());
        var minutes = new String(d.getMinutes());
        var seconds = new String(d.getSeconds());

        if (day.length == 1)
            day = "0" + day;

        if (month.length == 1)
            month = "0" + month;

        if (hours.length == 1)
            hours = "0" + hours;

        if (minutes.length == 1)
            minutes = "0" + minutes;

        if (seconds.length == 1)
            seconds = "0" + seconds;

        dformat = [month,
            day,
            d.getFullYear()
        ].join('/') + ' ' + [hours,
            minutes,
            seconds
        ].join(':');
        return dformat;
    }

    function sendRequest() {

        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/state/movimento", false);
        xhr.send();
        var resp = xhr.responseText;

        if (resp == "true") {
            const list = document.getElementById('events-list');
            const item = document.createElement('li');
            item.appendChild(document.createTextNode(`Evento: Movimento detectado - ${formatDate(new Date())}`));
            list.appendChild(item);
        }
    }

    function alternateTextOpacity() {
        let apply = true;
        return function() {
            console.log("applying " + apply);
            const sendingRequestText = document.getElementById('sending-request');
            if (apply) {
                sendingRequestText.classList.add('transition');
            } else {
                sendingRequestText.classList.remove('transition');
            }
            apply = !apply;
        }
    }
    addListenerGoHome();
    const alternateText = alternateTextOpacity();
    setInterval(() => {
        alternateText();
    }, 1000);
    setInterval(() => {
        sendRequest();
    }, 1200);
</script>

</html>
)=====";

const char HOME_PAGE[] = R"=====(
<html>

<head>
    <meta charset="UTF-8">
    <title>Home</title>
    <style>
        * {
            font-family: Arial, Helvetica, sans-serif;
            margin: 0;
            padding: 0;
            background-color: ghostwhite
        }
        
        .btn {
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
            border: 0;
            cursor: pointer;
        }
        
        .btn-on-off {
            color: #fff;
            width: 150px;
            text-align: center;
            padding: 6px 6px;
            border-radius: 6px;
            font-weight: 600;
            cursor: pointer;
            transition: all .6s;
            box-shadow: 0px 1px 5px 3px #184a07;
            background-color: #184a07;
        }
        
        .page {
            background-color: grey;
        }
        
        .button-container {
            width: 900px;
            display: flex;
            flex-flow: row wrap;
            justify-content: space-between;
            align-items: center;
        }
        
        section {
            width: 100%;
            height: 100%;
        }
        
        .container-all {
            display: flex;
            flex-flow: row wrap;
            justify-content: center;
            align-items: center;
            padding: 50px 50px;
        }
        
        .flex {
            display: flex;
            flex-flow: row wrap;
            justify-content: center;
            align-items: center;
        }
        
        .header {
            flex: 0 0 100%;
            font-weight: 600;
            font-size: 30px;
            font-style: italic;
            padding: 10px 80px;
        }
        
        .titles {
            flex: 0 0 100%;
            font-weight: 500;
            font-size: 18px;
            padding: 50px 50px;
        }
        
        #loading {
            background: no-repeat center center url("data:image/gif;base64,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");
            position: absolute;
            top: 0;
            left: 0;
            height: 100%;
            width: 100%;
            background-size: 60px;
            background-color: rgb(0, 0, 0, 0.5);
            visibility: hidden;
        }
    </style>
    <script>

        function turnOffAlarm() {
            var xhr = new XMLHttpRequest();
            xhr.open("POST", "/stop/alarme", false);
            xhr.send();
            var resp = xhr.responseText;

            if (resp == "alarme not active") {
                alert("Alarme não disparado.");
            }else if (resp == "parou") {
                alert("Alarme parado.");
            }
        }

        function turnOnAndOff() {

            var load = document.getElementById("loading");
            load.style.visibility = "visible";

            var xhr = new XMLHttpRequest();
            xhr.open("POST", "/onoff", false);
            xhr.send();
            var resp = xhr.responseText;
            load.style.visibility = "hidden";
            const button = document.getElementById('submit-btn');

            if (resp == "on") {
                button.style.backgroundColor = 'green';
                button.style.boxShadow = '0px 1px 5px 3px green';
            } else {

                button.style.backgroundColor = 'red';
                button.style.boxShadow = '0px 1px 5px 3px red';
            }
        }
    </script>
</head>

<body>
    <div id="loading"></div>
    <section>
        <div class="container-all">
            <div class="header flex">
                <h3>Sistema de Alerta Contra Invasão Inteligente</h3>
            </div>
            <div class="titles flex">
                <p>Monitoramento e Alarma de segurança para sua moradia controlado via App!</p>
            </div>

            <div class="button-container">
                <div class="button">
                    <button id="submit-btn" class="btn btn-on-off" onclick="turnOnAndOff()">On/Off</button>
                </div>
                <div class="button">
                    <button class="btn page" onclick="turnOffAlarm()">Desligar Alarme</button>
                </div>
                <div class="monitoring">
                    <button class="btn page" onclick="window.location.href = window.location.origin + '/monitor'">Monitores</button>
                </div>
                <div class="wifi">
                    <button class="btn page" onclick="window.location.href = window.location.origin + '/wifi'">Wifi
                        Config</button>
                </div>
            </div>
        </div>
    </section>
</body>

</html>
)=====";