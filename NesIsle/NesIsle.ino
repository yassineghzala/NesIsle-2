
// Sketch pour piloter la led integree a la Wemos D1 mini depuis le reseau WiFi
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
const char* ssid = "DESKTOP-IKNUPBG 5829";          // Identifiant du WiFi que vous utilisez
const char* password = "0r/54E52";  // Mot de passe WiFi que vous utilisez
ESP8266WebServer server(80);        // On demande un serveur qui ecoute sur le port 80



#define DHTPIN 13          // Pin which is connected to the DHT sensor.
#define DHTTYPE DHT11     // DHT 11
float t;
float h;
DHT dht(DHTPIN, DHTTYPE);



int led = 2; // La diode LED intégrée à la carte est branchée sur la broche GPIO2 (D4)
String  etatGPIO2 = "ON";


//Ecrire une page Web HTML
String WebPage(){
  String page =R"(
<!DOCTYPE html>
<html lang="fr-FR">
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <!-- <meta http-equiv="refresh" content="10" /> -->
    <title>SKETCH DEMO ESP8266 Web Server</title>
    <style>
      @import url("https://fonts.googleapis.com/css2?family=Poppins:ital,wght@0,600;0,700;0,800;0,900;1,600;1,700;1,800&display=swap");
      * {

        padding: 0;
        margin: 0;
        box-sizing: border-box;
        background-color:#F4F4F6;
        font-family:Bahnschrift;
      }
      a {
        color: inherit;
      }
      body {
        opacity: 85%;
        
        min-height: 100vh;

        color: #003B73;
      }
      #email,#password{
        width:500px;
        border-radius:4px;
        border:2px solid #003B73;
        margin:10px;
      }
      #submit_login{
        width:150px;
        border-radius:4px;
        border:2px solid #003B73;
        margin-top:10px;
        margin-left:10px;
      }
      #login_display{
        display:flex;
        flex-direction: column;
        align-items: center;
      }
      button {
        border-radius: 4px;
        outline: none;
        border: 1px solid #DB1F48;
        cursor: pointer;
        background-color: #DB1F48;
        color: #E8EEF1;
        padding:15px;
      }
      .button_in_middle{
        display:flex;
        margin-top:35%;
        justify-content:center;
      }
      #secured_page {
        display: none;
      }
      h1{
        color:#DB1F48;
      }
      #secured_page > h1 {
        
        margin-bottom: 60px;
        text-align: center;
      } 
      #secured_page{
        font-family: Bahnschrift;
      }
      #secured_page,
      #unsecured_page {
        
        max-width: 900px;
        padding: 20px;
        margin: auto;
      }
      #unsecured_page {
        margin-top: 60px;
      }
      form {
        display: flex;
        flex-direction: column;
        gap: 10px;
      }
      input {
        padding: 8px;
      }
      input[type="submit"] {
        cursor: pointer;
      }
      #user{
        color:#003B73;
      }
      #user-content {
        align-items: center;
        justify-content: space-between;
        display: flex;
        gap: 7px;
      }
      #loader {
        position: absolute;
        left: 0;
        top: 0;
        right: 0;
        bottom: 0;
        background-color: #E8EEF1;
        align-items: center;
        justify-content: center;
        flex-direction: column;
        display: flex;
      }
      .lds-facebook {
        display: inline-block;
        position: relative;
        width: 80px;
        height: 80px;
      }
      .lds-facebook div {
        display: inline-block;
        position: absolute;
        left: 8px;
        width: 16px;
        background: #fff;
        animation: lds-facebook 1.2s cubic-bezier(0, 0.5, 0.5, 1) infinite;
      }
      .lds-facebook div:nth-child(1) {
        left: 8px;
        animation-delay: -0.24s;
      }
      .lds-facebook div:nth-child(2) {
        left: 32px;
        animation-delay: -0.12s;
      }
      .lds-facebook div:nth-child(3) {
        left: 56px;
        animation-delay: 0;
      }
      @keyframes lds-facebook {
        0% {
          top: 8px;
          height: 64px;
        }
        50%,
        100% {
          top: 24px;
          height: 32px;
        }
      }
      .card{
        border-radius: 7px;
        padding: 2%;
        margin-left:50px;
        margin-top: 5%;
        border: solid #004369 4px;
        background-image: url(test.jpg);
        background-size: 900px 500px;
        box-shadow: rgba(0, 0, 0, 0.35) 0px 5px 15px;
        width:700px;
        }
      #iset_ch {
        color: #003B73;
        font-style: italic;
        position: absolute;
        bottom: 6px;
        left: 0;
        right: 0;

        text-align: center;
      }
      #user-email {
        color: #003B73;
      }
      #myForm {
        margin-top: 20px;
        padding-left: 20px;
      }
      #command-etat {
        display: flex;
        gap: 10px;
        align-items: center;
        justify-content: space-between;
      }
      .title {
        font-size: 500%;
        font-weight: bold;
        text-align: center;
        color: #DB1F48;
        
      }
      #message {
        height: 20px;
        text-align: center;
        color: rgb(235, 46, 46);
      }
      #etat {
        width: 100px;
      }
      .color-orange {
        color: #003B73;
      }
    </style>
    <!-- Load the Firebase SDKs -->

    <script type="module">
      import { initializeApp } from "https://www.gstatic.com/firebasejs/9.17.2/firebase-app.js";
      import {
        getAuth,
        onAuthStateChanged,
        signInWithEmailAndPassword,
        signOut,
      } from "https://www.gstatic.com/firebasejs/9.17.2/firebase-auth.js";

    // TODO: Add SDKs for Firebase products that you want to use
    // https://firebase.google.com/docs/web/setup#available-libraries

    // Your web app's Firebase configuration
    const firebaseConfig = {
  apiKey: "AIzaSyDwRa0eay2B4-0VVmOsz4iA7r5U9VPyPQ0",
  authDomain: "nesisle.firebaseapp.com",
  projectId: "nesisle",
  storageBucket: "nesisle.appspot.com",
  messagingSenderId: "706593100103",
  appId: "1:706593100103:web:2e250f262d4399e5a77c62",
  measurementId: "G-JWTJJE7NXW"
};

    // Initialize Firebase
    const app = initializeApp(firebaseConfig);
      const auth = getAuth(app);

      const loginForm = document.getElementById("login-form");
      const submit_login_btn = document.getElementById("submit_login");
      const message = document.getElementById("message");

      loginForm.addEventListener("submit", (e) => {
        e.preventDefault();
        submit_login_btn.value = "Login...";
        submit_login_btn.disabled = true;
        // submit_login_btn.value = "login...";
        // submit_login_btn.disabled = true;
        message.value = "You are logged in";
        const email = loginForm.email.value;
        const password = loginForm.password.value;

        signInWithEmailAndPassword(auth, email, password)
          .then((userCredential) => {
            // User signed in successfully...
            const user = userCredential.user;
            console.log("Logged in user:", user);
            submit_login_btn.value = "Login";
            submit_login_btn.disabled = false;
          })
          .catch((error) => {
            // Handle login errors...
            console.error("Login failed:", error);
            message.innerText = "invalid email or password";
            submit_login_btn.value = "Login";
            submit_login_btn.disabled = false;
          });
      });
      const loader = document.getElementById("loader");
      const secured_page = document.getElementById("secured_page");
      const unsecured_page = document.getElementById("unsecured_page");
      const user_email_div = document.getElementById("user-email");

      onAuthStateChanged(auth, (user) => {
        if (user) {
          // User is logged in, show the protected form
          loader.style.display = "none";
          secured_page.style.display = "block";
          unsecured_page.style.display = "none";
          var userEmail = user.email;
          user_email_div.innerHTML = userEmail;
        } else {
          // User is not logged in, hide the protected form
          loader.style.display = "none";
          secured_page.style.display = "none";
          unsecured_page.style.display = "block";
        }
      });

      // Handle logout button click
      const logoutButton = document.getElementById("logoutButton");

      logoutButton.addEventListener("click", () => {
        signOut(auth);
      });




      
      const myForm = document.getElementById('myForm');
      const radioButtons = document.querySelectorAll('input[type=\"radio\"]');
      radioButtons.forEach((radioButton) => {
        radioButton.addEventListener('change', () => {
          myForm.submit();
        });
      });
      myForm.onsubmit((e) => {
        e.preventDefault();
      });
    </script>
</head>
  <body>
    <div id="loader">
      <h2>connecting to <span class="color-orange">firebase</span></h2>
      <div class="lds-facebook">
        <div></div>
        <div></div>
        <div></div>
      </div>
    </div>
    <div id="secured_page">
      <h1>ISET CHARGUIA</h1>
      <form method="POST" id="myForm" name="myForm">
        <div id="command-etat">
          <h3>Commande d'une LED sur GPIO</h3>
          <div id="etat">GPIO2 : <span>)";
            page+=etatGPIO2;
            page+=R"(
              </span></div>
        </div>
        <label>
          <input
            type="radio"
            name="GPIO2"
            value="1"
     
            id="ON"
          )";
          if (etatGPIO2 == "ON"){
            page+="checked";
            }
            page+=R"(
            />
            <span>ON</span>
          </label>
          <label>
            <input type='radio' name='GPIO2' value='0')"; 
            if (etatGPIO2 == "OFF"){
              page+="checked";
              }
              page+=R"(
              />
          <span> OFF</span>
        </label>)";
         String message = "Temperature: " + String(dht.readTemperature()) + "C | Humidity: " + String(dht.readHumidity()) + "%";

  page += R"(
    <div id='temperature-humidity'>
      <p>)";
  page += message;
  page += R"(</p>
    </div>
      </form>
      <div class="button_in_middle">
        <button id="logoutButton">logout</button>
      <div/
      <br /><br />
      <p id="iset_ch"><a href="#">ISET Charguia &copy;</a></p>
    </div>
    <div id="unsecured_page">
      <div class="card">
        <form method="POST" id="login-form">
          <h1 class="title">LOGIN</h1>
          <div id="message"></div>
          <div id="login_display">
            <label for="email" id="label_text">Email</label>
            <input type="email" name="email" id="email" required />
            <label for="password" id="label_text">Password</label>
            <input type="password" name="password" id="password" required />
            <input type="submit" value="login" id="submit_login" />
          </div>
        </form>
      </div>
    </div>
  </body>
</html>)";

return page;
}



void Soumettre() {
  // Actualise le GPIO 
  String GPIOValue;
  GPIOValue = server.arg("GPIO2");
  Serial.print("Set GPIO to "); Serial.println(GPIOValue);
  if ( GPIOValue == "1" ) {
    digitalWrite(2, 0);
    etatGPIO2 = "ON";
    server.send ( 200, "text/html", WebPage() );
  } else if ( GPIOValue == "0" ) {
    digitalWrite(2, 1);
    etatGPIO2 = "OFF";
    server.send ( 200, "text/html", WebPage() );
  } else {
    Serial.println("Erreur Valeur LED");
  }
}
void Racine(){ 
  if ( server.hasArg("GPIO2") ) {
    Soumettre();
  } else {
    server.send ( 200, "text/html", WebPage() );
  }  
}
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
  dht.begin();

  
  // on attend d'etre connecté au WiFi avant de continuer
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  // on affiche l'adresse IP qui nous a ete attribuée
  Serial.println("");
  Serial.print("Addresse IP : ");
  Serial.println(WiFi.localIP());
  // on definit les points d'entree (les URL a saisir dans le navigateur web) et on affiche un simple texte
  server.on ( "/", Racine );

  // on demarre le serveur web
  server.begin();
  pinMode(led, OUTPUT);     // Initialise la broche GPIO2 comme une sortie
}

// Cette boucle s'exécute à l'infini
void loop() {
    // a chaque iteration, on appelle handleClient pour que les requetes soient traitees
  server.handleClient();
   delay(2000);  // Wait a few seconds between measurements.

   h = dht.readHumidity();
   t = dht.readTemperature();

  String message = "Temperature: " + String(t) + "C | Humidity: " + String(h) + "%";
  server.send(200, "text/plain", WebPage());

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");
}
void handleRoot() 
{
  String htmlpage = WebPage();
  htmlpage.replace("%%HUMIDITY%%", String(h));
  htmlpage.replace("%%TEMPERATURE%%", String(t));

  server.send(200, "text/html", htmlpage);
}
