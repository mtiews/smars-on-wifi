#ifndef WEBCONTROL_H
#define WEBCONTROL_H

#include <ESP8266WebServer.h>

class Webcontrol {
  public:
    Webcontrol(
      int port,
      void (*moveForwardHandler)(void), 
      void (*moveLeftHandler)(void), 
      void (*moveRightHandler)(void), 
      void (*moveBackwardHandler)(void), 
      void (*moveStopHandler)(void),
      void (*enableAutomodeHandler)(void)
    );
    void begin();
    void handleClientCommands();
  private:
    ESP8266WebServer *_webServer;
    void (*_moveForwardHandler)(void); 
    void (*_moveLeftHandler)(void);
    void (*_moveRightHandler)(void);
    void (*_moveBackwardHandler)(void); 
    void (*_moveStopHandler)(void);
    void (*_enableAutomodeHandler)(void);
    
    void _handleRoot();
    void _handleNotFound();
    void _sendStatus(String status);
    void _moveForward();
    void _moveLeft();
    void _moveStop();
    void _moveRight();
    void _moveBackward();
    void _enableAutomode();
};

const char WEBCONTROL_PAGE[] PROGMEM = R"=====(
<html class="full">
    <head>
        <style>
            .full {
                width: 100%; 
                height: 100%; 
                font-family: verdana;
            }
            td {
                width: 33%;
                padding: 2px;
            }
            tr {
                padding: 0px;
            }
            .button {
                width: 100%;
                height: 100%;
                font-size: 8vw;
                text-align: center;
                cursor: pointer;
                margin: 0 auto;
                background: #EE3333;
                border: 3px solid #CC2222;
                border-radius: 1vw;
                box-shadow: none;
                outline: 0;
            }

            .lightbutton {
                width: 100%;
                height: 100%;
                font-size: 8vw;
                text-align: center;
                cursor: pointer;
                margin: 0 auto;
                background: #EE7733;
                border: 3px solid #CC6622;
                border-radius: 1vw;
                box-shadow: none;
                outline: 0;
            }
            #status {
                font-size: 3vw;
            }
        </style>
        <script type="text/javascript">
            function sendCommand(command) {
                var req = new XMLHttpRequest();
                req.onreadystatechange = function() {
                    if (req.readyState == 4) {
                        var se = document.getElementById('status');
                        if(req.status != 200) {
                            alert('Request Failed: ' + req.status + '' + req.responseText);
                            se.textContent = 'Error: ' + req.status + '' + req.responseText;
                        } else {
                          se.textContent = 'Status: ' + req.responseText;
                        }
                    }
                };
                req.open('GET', '/command/' + command);
                req.send();
            }
        </script>
    </head>
    <body class="full">
        <table class="full">
            <tr>
                <td></td>
                <td><button class="button" onClick="javascript:sendCommand('forward')">&#8679;</button></td>
                <td class="nobutton"></td>
            </tr>
            <tr>
                <td><button class="button" onClick="javascript:sendCommand('left')">&#8678;</button></td>
                <td><button class="button" onClick="javascript:sendCommand('stop')">STOP</button></td>
                <td><button class="button" onClick="javascript:sendCommand('right')">&#8680;</button></td>
            </tr>
            <tr>
                <td><span id="status"></span></td>
                <td><button class="button" onClick="javascript:sendCommand('backward')">&#8681;</button></td>
                <td><button class="lightbutton" onClick="javascript:sendCommand('auto')">AUTO</button></td>
            </tr>      
        </table>
    </body>
</html>
)=====";

#endif
