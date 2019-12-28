#define QUOTE(...) #__VA_ARGS__
char * index_page = QUOTE(
	
<meta charset='utf-8'>
<html>
<head>
<title>Config WiFi</title>
</head>
<body>

<form action='./'>
Tên wifi:  <input type='text' name='ssid' id = 'wfname' />
<br><br>
Mật khẩu wifi:  <input type='password' name='sspass' />
<br><br>
<input type='submit' value='Xong'>
</form>

<form action='./update'>
<input type='submit' value='Nạp chương trình'>
</form>
<form action='./config'>
<input type='submit' value='Config Hệ Thống'>
</form>
<input type="checkbox" id = "nearWF" value="Bike" onclick="getData()"> Near WiFi<br>
<div id= 'listWifi'>	
</div>
</body>
<script>
function getData() {
		if(document.getElementById("nearWF").checked == true)
		{
			var xhttp = new XMLHttpRequest();
			xhttp.onreadystatechange = function () {
				if(this.readyState == 4 && this.status== 200){
					
					var resText=xhttp.responseText.split('|');
					var tmp='';
					for (i = 0; i < resText.length; i++) {
							tmp=tmp+'<div id = '+i+ ' onclick="setUsr(this)">' + resText[i] +  '</div>' ;
						}
						document.getElementById('listWifi').innerHTML=tmp;
					getData();
				}
			};
			xhttp.open('GET','/listWifi',true);
			xhttp.send();
		}
		else{
			document.getElementById('listWifi').innerHTML="";
		}
		
	};
function setUsr(divObj){
	document.getElementById('wfname').value = divObj.innerHTML;
}
</script>
</html>

);

