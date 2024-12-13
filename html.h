const char home[] PROGMEM = R"rawliteral(
<HTML>
<HEAD>
    <TITLE>ReadyTouch 2.8</TITLE>
    <style>
        body {
            background-color: #fff;
            font-family: sans-serif;
            color: #333333;
            font: 14px Helvetica, sans-serif;
            box-sizing: border-box;
        }

        #page {
            margin: 20px;
            background-color: #ffffff;
        }

        .container {
            height: inherit;
            padding-bottom: 20px;
        }

        #content {
            background: #e1e1e1;
            border-radius: .5em .5em .5em .5em;
            box-shadow: 1px 7px 7px 1px rgba(0, 0, 0, 0.4);
            display: block;
        }

            #content:after {
                content: " ";
                display: block;
                height: 0;
                clear: both;
            }

        .header {
            padding: 20px;
        }

            .header h1 {
                padding-bottom: 0.3em;
                font-size: 45px;
                font-weight: normal;
                font-family: Garmond,"sans-serif";
                text-align: left;
                margin: 0;
            }

                .header h1 span {
                    font-weight: bold;
                    font-family: Garmond,"sans-serif";
                    color: #000000;
                }

        h2 {
            padding-bottom: 0.2em;
            border-bottom: 1px solid #eee;
            margin-top: 2px;
        }

        .box380 {
            width: 380px;
            padding: 20px;
            margin: 10px 10px 10px 10px;
            border: 1px solid #ddd;
            border-radius: 1em 1em 1em 1em;
            box-shadow: 1px 7px 7px 1px rgba(0, 0, 0, 0.4);
            background: #ffffff;
        }

        .left {
            float: left;
            clear: left;
        }

        .right {
            float: right;
            clear: right;
        }

        .top_left {
            font-size: 9px;
            display: inline-block;
            vertical-align: top;
        }

        @media (min-width: 980px) {
            #page {
                width: 900px;
                margin: 0 auto;
            }
        }

        table {
            width: 100%;
            border-collapse: collapse;
        }

        th {
            background-color: #777777;
            color: #ffffff;
            border: 1px solid #f1f1f1;
            padding: 6px;
        }

        td {
            color: #777777;
            border: 1px solid #e1e1e1;
            padding: 4px;
        }

        tr {
            background-color: #ffffff;
        }

        input:optional {
            border-color: #ddd;
        }

        input[type="text"],
        input[type="number"],
        input[type="password"],
        input[type="file"],
        input[type="time"],
        input[type="date"],
        select {
            box-sizing: border-box;
            border: 1px solid #ddd;
            border-radius: 4px;
            margin-top: 0px;
            width: 100%;
            padding: 5px;
        }

        button {
            border-radius: 5px;
            box-shadow: 0 2px 2px 0 rgba(0, 0, 0, 0.4);
            border: none;
            color: #ffffff;
            padding: 10px 28px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 1em;
            margin: 4px 2px;
            cursor: pointer;
            outline: none;
            background-color: rgb(105, 143, 0);
        }

        #wifiList {
            display: inline-block;
            width: 100%;
            max-height: 230px;
            overflow: auto
        }

        .list-group {
            display: flex;
            flex-direction: column;
            padding-left: 0;
            margin-bottom: 0;
        }

        .list-group-item {
            position: relative;
            display: flex;
            padding: .75rem 1.25rem;
            margin-bottom: -1px;
            background-color: #fff;
            border: 1px solid rgba(0,0,0,.125);
            justify-content: space-between;
            align-items: center;
        }

            .list-group-item.active {
                z-index: 2;
                color: rgb(105, 143, 0);
            }

            .list-group-item:first-child {
                border-top-left-radius: .25rem;
                border-top-right-radius: .25rem;
            }

            .list-group-item:last-child {
                margin-bottom: 0;
                border-bottom-right-radius: .25rem;
                border-bottom-left-radius: .25rem;
            }
        .loader {
            z-index: 10000;
            border: 8px solid #b5b5b5; /* Grey */
            border-top: 8px solid #3498db; /* Blue */
            border-bottom: 8px solid #3498db; /* Blue */
            border-radius: 50%;
            width: 240px;
            height: 240px;
            animation: spin 2s linear infinite;
            display: none;
        }
       #footer {
            font-size: 15px;
            text-align: center;
            margin-bottom: 0;
        }
    </style>
</HEAD>
<BODY onload="onBodyLoad()">
    <div id="page">
        <div class="header">
            <h1 style="color:rgb(105, 143, 0)">
                Ready Touch <span>2.8</span>
            </h1>
        </div>
        <div class="container">
            <div id="content">
                <div class="box380 left">
                    <h2>Inputs</h2>
                    <table>
                        <tr>
                            <th>Input</th>
                            <th>Value</th>
                        </tr>
                        <tr>
                            <td>
                                <span class="top_left">1</span>
                                <span>In 1</span>
                            </td>
                            <td>
                                <span id="in_0"></span>v
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <span class="top_left">2</span>
                                <span>In 2</span>
                            </td>
                            <td>
                                <span id="in_1"></span>v
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <span class="top_left">3</span>
                                <span>vbus</span>
                            </td>
                            <td>
                                <span id="vbus"></span>v
                            </td>
                        </tr>
                    </table>
                </div>
                <div class="box380 right">
                    <h2>WiFi Setup</h2>
                    <p>
                        <b>Mode: </b><span id="mode"></span>
                    </p>
                    <div id="wired_view" style="display: none">
                        <table>
                            <tr>
                                <th colspan="2">Network</th>
                            </tr>
                            <tbody>
                                <tr>
                                    <td>IP Address:</td>
                                    <td data-bind="text: config.ipaddress(), attr: {href: 'http://'+config.ipaddress()}"></td>
                                </tr>
                                <tr>
                                    <td>Gateway:</td>
                                    <td data-bind="text: config.gw()"></td>
                                </tr>
                                <tr>
                                    <td>DNS:</td>
                                    <td data-bind="text: config.dns()"></td>
                                </tr>
                                <tr>
                            </tbody>
                        </table>
                    </div>
                    <div id="sta_view">
                        <table>
                            <tr>
                                <th>Network</th>
                                <th>RSSI dBm</th>
                                <th>Channel</th>
                            </tr>
                            <tr>
                                <td><span id="ssid"></span></td>
                                <td><span id="srssi"></span>&nbsp;&nbsp;&nbsp;&nbsp;<span id="dbm"></span>%</td>
                                <td><span id="chan"></span></td>
                            </tr>
                            <tr>
                                <td>BSSID:</td>
                                <td colspan="2"><span id="bssid"></span></td>
                            </tr>
                            <tr>
                                <td>TX Power:</td>
                                <td colspan="2"><span id="txpwr"></span>&nbsp;dBm</td>
                            </tr>
                            <tr>
                                <td>Authentication Mode:</td>
                                <td colspan="2" data-bind="text: config.eauthmode()"></td>
                            </tr>
                        </table>
                        <button id="wifiChange">Change WiFi network</button>
                        <button data-bind="click: setAPMode, text: (saveAPModeFetching() ? 'Saving' :  (saveAPModeSuccess() ? 'Saved' : 'AP mode')), disable: saveAPModeFetching, style: {'background-color': config.web_color}">AP mode</button>
                        <br />
                        <br />
                        <table>
                            <tr>
                                <th colspan="2">Network</th>
                            </tr>
                            <tbody>
                                <tr>
                                    <td>IP Address:</td>
                                    <td><span id="ip"></span></td>
                                </tr>
                                <tr>
                                    <td>Gateway:</td>
                                    <td><span id="gw"></span></td>
                                </tr>
                                <tr>
                                    <td>DNS:</td>
                                    <td><span id="dns"></span></td>
                                </tr>
                            </tbody>
                        </table>
                        <button id="apoff" data-bind="visible: status.isWifiAccessPoint(), click: wifi.turnOffAccessPoint, disable: wifi.turnOffAccessPointFetching, style: {'background-color': config.web_color}">Turn off Access Point</button>
                    </div>
                    <div id="ap_view" style="display: none">
                        <table>
                            <tr>
                                <th>Network</th>
                                <th>Channel</th>
                            </tr>
                            <tr>
                                <td id="apssid"></td>
                                <td id="apchan"></td>
                            </tr>
                            <tr>
                                <td>TX Power:</td>
                                <td data-bind="text: config.txpwr() + ' dBm'"></td>
                            </tr>
                        </table>
                        <button id="btn_wifiChange2">Change WiFi network</button>
                    </div>
                    <div id="ap_scan_view" style="display: none">
                        <p>Connect to network:</p>
                        <div id="wifiList">
                            <ul class="list-group" id="scanList">
                            </ul>
                        </div>
                        <div data-bind="visible: 0 == scan.results().length">
                            Scanning...
                        </div>
                        <p>
                            <b>SSID:</b><br>
                            <input id="sta_ssid" type="text" autocapitalize="none">
                        </p>
                        <p>
                            <b>Passkey:</b><br>
                            <input type="password" autocapitalize="none" id="sta_pass"><br />
                            <div>
                                <input id="chk_pass" type="checkbox" onclick="togglePass()">
                                <label for="chk_pass">Show password</label>
                            </div>
                        </p>
                        <p>
                            <button id="wifiSave">Connect</button>
                        </p>
                    </div>

                    <div id="wifiConnecting" style="display: none">
                        <p>Connecting to WIFI Network...</p>
                    </div>
                </div>
            </div>
            <div id="loader" class="loader"></div>
            <div id="footer">
                <br>
                <b>Powered by <a href="http://WeGoWireless.com.com">WeGoWireless.com</a></b>
                <br>
                <b>Version: </b>V<span id="appVersion"></span>
            </div>
        </div>
        <script type="text/javascript">
            if (typeof XMLHttpRequest === "undefined") {
                XMLHttpRequest = function () {
                    try { return new ActiveXObject("Msxml2.XMLHTTP.6.0"); } catch (e) { }
                    try { return new ActiveXObject("Msxml2.XMLHTTP.3.0"); } catch (e) { }
                    try { return new ActiveXObject("Microsoft.XMLHTTP"); } catch (e) { }
                    throw new Error("This browser does not support XMLHttpRequest.");
                };
            }

            // If serving from ESP32
            var baseHost = window.location.hostname;
            var basePort = window.location.port;
            var baseProtocol = window.location.protocol;
            var endpoint = "//" + baseHost;

            //If serving from file baseProtocol is file:/ /
            //var baseHost = "10.10.10.47";
            //var basePort = 80;
            //var baseProtocol = "http";
            //var endpoint = baseProtocol + "://" + baseHost;

            var reloadPeriod = 3000;
            var settingRunning = false;

            var scanPeriod = 5000;
            var scanRunning = false;

            function ge(a) {
                return document.getElementById(a);
            }
            function ce(a) {
                return document.createElement(a);
            }

            function loadValues() {
                if (!settingRunning) return;
                var xh = new XMLHttpRequest();
                xh.onreadystatechange = function () {
                    if (xh.readyState == 4) {
                        if (xh.status == 200) {
                            var res = JSON.parse(xh.responseText);

                            appVersion.innerText = res.appVersion;
                            in_0.innerText = res.in_0;
                            in_1.innerText = res.in_1;
                            vbus.innerText = res.vbus;

                            if (ge('ap_scan_view').style.display != "block") {
                                mode.innerText = res.mode;
                                if (res.mode == "ETH") {
                                    ge('wired_view').style.display = "block";
                                    ge('sta_view').style.display = "none";
                                    ge('ap_view').style.display = "none";
                                } else if (res.mode == "STA") {
                                    ge('wired_view').style.display = "none";
                                    ge('sta_view').style.display = "block";
                                    ocument.getElementById('ap_view').style.display = "none";
                                } else if (res.mode == "AP") {
                                    ge('wired_view').style.display = "none";
                                    ge('sta_view').style.display = "none";
                                    ocument.getElementById('ap_view').style.display = "block";
                                } else if (res.mode == "STA+AP") {
                                    ge('wired_view').style.display = "none";
                                    ge('sta_view').style.display = "block";
                                    ge('ap_view').style.display = "block";
                                }
                            }

                            srssi.innerText = res.srssi;
                            var percentRSSI = res.srssi >= -50 ? 100 : res.srssi <= -100 ? 0 : (2 * res.srssi) + 100;
                            dbm.innerText = percentRSSI;
                            ssid.innerText = res.ssid;
                            bssid.innerText = res.bssid;
                            chan.innerText = res.chan;
                            txpwr.innerText = res.txpwr;
                            ip.innerText = res.ip;
                            gw.innerText = res.gw;
                            dns.innerText = res.dns;
                            sta_ssid.value = res.sta_ssid;
                            sta_pass.value = res.sta_pass;

                            if (settingRunning) setTimeout(loadValues, reloadPeriod);
                        } else settingRunning = false;
                    }
                };
                console.log(endpoint);
                xh.open("GET", endpoint + "/status", true);
                xh.send(null);
            }

            function strength(val) {
                if (val >= -50) {
                    return 5;
                }
                if (val >= -60) {
                    return 4;
                }
                if (val >= -67) {
                    return 3;
                }
                if (val >= -70) {
                    return 2;
                }
                if (val >= -80) {
                    return 1;
                }
                return 0;
            }

            function loadScan() {
                if (!scanRunning) return;
                var xh = new XMLHttpRequest();
                xh.onreadystatechange = function () {
                    if (xh.readyState == 4) {
                        if (xh.status == 200) {
                            scanResults = JSON.parse(xh.responseText);

                            if (scanResults.length > 0) {
                                if (scanList.hasChildNodes()) {
                                    while (scanList.hasChildNodes()) {
                                        scanList.removeChild(scanList.firstChild);
                                    } 
                                }
                                for (i = 0; i <= scanResults.length - 1; i++) {
                                    const scanLi = ce('li');
                                    const scanSpan = ce('span');
                                    const scanImg = ce('img');

                                    scanLi.classList.add("list-group-item");
                                    if (scanResults[i].ssid === sta_ssid.value) {
                                        scanLi.classList.add("active");
                                    }
                                    scanLi.setAttribute('style', 'display: block;');
                                    scanLi.setAttribute('onclick', 'selectedNet(this.innerText);')
                                    scanSpan.innerHTML = scanResults[i].ssid;
                                    scanLi.appendChild(scanSpan);

                                    scanImg.setAttribute('width', '24px');
                                    scanImg.setAttribute('heigth', '24px');
                                    // from ESP
                                    scanImg.setAttribute('class', 'wifi_signal_' + strength(scanResults[i].rssi));
                                    // from file
                                    //scanImg.setAttribute('src', 'wifi_signal_' + strength(scanResults[i].rssi) + '.svg');
                                    scanImg.setAttribute('style', 'float: right;');
                                    scanLi.appendChild(scanImg);

                                    scanList.appendChild(scanLi);
                                }
                            }
                            if (scanRunning) setTimeout(loadScan, reloadPeriod);
                        } else scanRunning = false;
                    }
                };
                xh.open("GET", endpoint + "/scan", true);
                xh.send(null);
            }

            function run() {
                if (!settingRunning) {
                    settingRunning = true;
                    loadValues();
                }
            }

            function run_WiFi_Scan() {
                if (!scanRunning) {
                    settingRunning = false;
                    scanRunning = true;
                    loadScan();
                }
            }

            var QueuedRequester = function () {
                this.queue = [];
                this.running = false;
                this.xmlhttp = null;
            }
            QueuedRequester.prototype = {
                _request: function (req) {
                    this.running = true;
                    if (!req instanceof Object) return;
                    var that = this;

                    function ajaxCb(x, d) {
                        return function () {
                            if (x.readyState == 4) {
                                ge("loader").style.display = "none";
                                d.callback(x.status, x.responseText);
                                if (that.queue.length === 0) that.running = false;
                                if (that.running) that._request(that.queue.shift());
                            }
                        }
                    }

                    ge("loader").style.display = "block";

                    var p = "";
                    if (req.params instanceof FormData) {
                        p = req.params;
                    } else if (req.params instanceof Object) {
                        for (var key in req.params) {
                            if (p === "")
                                p += (req.method === "GET") ? "?" : "";
                            else
                                p += "&";
                            p += encodeURIComponent(key) + "=" + encodeURIComponent(req.params[key]);
                        };
                    }

                    this.xmlhttp = new XMLHttpRequest();
                    this.xmlhttp.onreadystatechange = ajaxCb(this.xmlhttp, req);
                    if (req.method === "GET") {
                        this.xmlhttp.open(req.method, req.url + p, true);
                        this.xmlhttp.send();
                    } else {
                        this.xmlhttp.open(req.method, req.url, true);
                        if (p instanceof String)
                            this.xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
                        this.xmlhttp.send(p);
                    }
                },
                stop: function () {
                    if (this.running) this.running = false;
                    if (this.xmlhttp && this.xmlhttp.readyState < 4) {
                        this.xmlhttp.abort();
                    }
                },
                add: function (method, url, params, callback) {
                    this.queue.push({ url: url, method: method, params: params, callback: callback });
                    if (!this.running) {
                        this._request(this.queue.shift());
                    }
                }
            }
            var requests = new QueuedRequester();

            function httpPostProcessRequest(status, responseText) {
                if (status != 200)
                    alert("ERROR[" + status + "]: " + responseText);
                //else
                    //tree.refreshPath(path.value);
            }

            function onBodyLoad() {
                const appVersion = ge("appVersion");

                const in_0 = ge("in_0");
                const in_1 = ge("in_1");
                const vbus = ge("vbus");

                const mode = ge("mode");
                const srssi = ge("srssi");
                const dbm = ge("dbm");
                const ssid = ge("ssid");
                const bssid = ge("bssid");
                const chan = ge("chan");
                const txpwr = ge("txpwr");
                const ip = ge("ip");
                const gw = ge("gw");
                const dns = ge("dns");
                const sta_ssid = ge("sta_ssid");
                const sta_pass = ge("sta_pass");

                const btn_wifiChange = ge("wifiChange");

                const scanList = ge("scanList");

                btn_wifiChange.onclick = function (e) {
                    ge('wired_view').style.display = "none";
                    ge('sta_view').style.display = "none";
                    ge('ap_view').style.display = "none";
                    ge('ap_scan_view').style.display = "block";
                    run_WiFi_Scan();
                }

                const btn_wifiSave = ge("wifiSave");
                btn_wifiSave.onclick = function (e) {
                    btn_wifiSave.innerText = 'Saving';
                    if (sta_ssid === "") {
                        alert("Please select network");
                    } else {
                        var formData = new FormData();
                        formData.append("ssid", sta_ssid.value);
                        formData.append("pass", sta_pass.value);
                        requests.add("POST", endpoint + "/savenetwork", formData, httpPostProcessRequest);

                        //.post(endpoint + "/savenetwork", { ssid: sta_ssid, pass: sta_pass }, function () {
                            // HACK: Almost certainly won't get a status update with client connected set to false so manually clear it here
                            //self.status.wifi_client_connected(false);

                            // Done with setting the config
                            //self.forceConfig(false);

                            // Wait for a new WiFi connection
                            //self.wifiConnecting(true);

                            // And indiccate the save was successful
                            //self.saveNetworkSuccess(true);
                        //}).fail(function () {
                        //    alert("Failed to save WiFi config");
                        //}).always(function () {
                       //     self.saveNetworkFetching(false);
                       //});
                    }
                }
                run();
            }

            function togglePass() {
                const x = ge("sta_pass");
                if (x.type === "password") {
                    x.type = "text";
                } else {
                    x.type = "password";
                }
            }

            function selectedNet(val) {
                sta_ssid.value = val;
            }

            function authmode_type(val) {
                var ret;
                switch (val) {
                    case 0:
                        ret = "Open";
                        break;
                    case 1:
                        ret = "WEP";
                        break;
                    case 2:
                        ret = "WPA PSK";
                        break;
                    case 3:
                        ret = "WPA2 PSK";
                        break;
                    case 4:
                        ret = "WPA WPA2 PSK";
                        break;
                    case 5:
                        ret = "WPA2 Enterprise";
                        break;
                    case 6:
                        ret = "WPA3 PSK";
                        break;
                    case 7:
                        ret = "WPA2 WPA3 PSK";
                        break;
                    case 8:
                        ret = "WAPI PSK";
                        break;
                    case 9:
                        ret = "MAX";
                        break;
                    default:
                        ret = "Unknown";
                        break;
                }
                return ret;
            }
        </script>
</BODY>
</HTML>
)rawliteral";