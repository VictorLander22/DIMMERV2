const char webDefaultPage[] PROGMEM = R"===(
<!DOCTYPE html><head><meta charset="UTF-8" name="viewport" content="width=device-width"><title>Keepin</title></head>
<style>body{background-color:#b3b3b3}input,select{width:95vw;margin:3px;height:30px;font-size:20px}.b,#fuso,select{width:95.5vw;height:40px;}label{font-weight:bolder}#dhcp,#fixo{height:20px;width:30px;}</style>
<body><h2>Configuração de rede</h2><form action="gravarwifi">
<label>SSID<br><input required type="text" name="txtnomerede" id="ssid"><br>Password<br><input required type="text" name="txtsenha" id="pwd"><br>
Fuso Horário<br><select name="utcsel" id="fuso">
<option value="-3">(-3:00) Brazil</option>
</select><table><tr><th>IP</th>
<th><input type="radio" name="ipfixo" onclick="isFixIp('dhcp',0)" value="0" id="dhcp"></th>
<th>Dinânico</th><th><input type="radio" name="ipfixo" onclick="isFixIp('fixo',1)" value="1" id="fixo"></th>
<th>Estático</th></tr></table>
<input required type="text" name="txtip" id="ip"><br>
Máscara<br><input required type="text" name="txtmascara" id="msk"><br>
Gateway<br><input required type="text" name="txtgateway" id="gtw"><br><br>
<input type="submit" class="b" value="Gravar" /></label></form>
<input type="submit" class="b" value="Reiniciar" onclick="window.location='/reiniciar'"/>
<input type="submit" class="b" value="Configuração de Fábrica" onclick="window.location='/reset'"/>
<input type="submit" class="b" value="Atualizar Firmware" onclick="window.location='/executeupdate?s=kdi9e'"/><script>
var utc =#utc#, ipfixo =#ipfixo#, ssid = "#ssid#", pwd = "#pwd#", ip = "#ip#", msk = "#msk#", gtw = "#gtw#";
window.onload = (function () {
fValue("fuso", utc); fValue("ssid", ssid); fValue("pwd", pwd); fValue("ip", ip); fValue("msk", msk); fValue("gtw", gtw);
ipfixo ? isFixIp("fixo", ipfixo) : isFixIp("dhcp", ipfixo);})
function isFixIp(v, t) { document.getElementById(v).checked = true; fDesab("ip", !t); fDesab("msk", !t); fDesab("gtw", !t); }
function fDesab(v, t) { document.getElementById(v).disabled = t; }
function fValue(v, t) { document.getElementById(v).value = t; }
</script></body></html>
)===";

// const char webAbout[] PROGMEM = R"===(
// <head><meta charset="UTF-8" name="viewport" content="width=device-width"><title>Keepin</title></head>
// <html><body><h1>Desenvolvido por:</h1><p>Leonardo Aquino</p><p>Julio Valadares</p><p>Vinícus Aquino</p><p>Breno Aquino</p><p><a href=\"http://www.keepin.com.br\">www.keepin.com.br</a></p></body></html>)===";

const char webRestart[] PROGMEM = R"===(
<h1>Redirecting to #newip# after <span id="countdown">15</span> seconds</h1>
<script type="text/javascript">var seconds = 15;function countdown() {seconds = seconds - 1;
if (seconds < 0) {window.location = "http://#newip#";} else {document.getElementById("countdown").innerHTML = seconds;window.setTimeout("countdown()", 1000);}}countdown();</script>
)===";

const char webUpload[] PROGMEM = R"===(
<h3>Select File to Upload</h3><FORM action='/fupload' method='post' enctype='multipart/form-data'><input type="file" id="upload" name="upload"><br><br><button class='buttons' style='width:10%' type='submit'>Upload File</button><br><br><a href='/'>[Back]</a><br><br>
)===";
