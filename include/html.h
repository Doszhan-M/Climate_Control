const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html style="margin: 0; padding: 0;">

    <head>
        <title>Climate Control Center</title>
        <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0">

    <style>
        .flex_center{
            display: flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
        }
        html {
            margin: 0;
            padding: 0;
            background-color: aquamarine;
            height: 100%;
        }
        body {
            margin: 0;
            padding: 0;
            height: 100%;
            width: 100%;
        }
        header {      
            flex: 0 0 auto;      
            height: 50px;
            width: 100%;
            background-color: blueviolet;
            margin-bottom: 10%;
            color: aquamarine;
        }
        .main-content{
            width: 100%;
            flex: 1 0 auto;
        }
        .temp_form {
            display: flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
        }
        .temp_form div {
            margin-bottom: 3%;
            font-size: 22px;
            font-weight: 500;
        }
        .btn {
            background-color: rgb(243, 228, 17);
            border-radius: 10%;
            height: 30px;
            width: 150px;
        }
        input {
            text-align: center;
            border-radius: 10%;
            height: 20px;
            width: 150px;
        }
        .circle_block {
            width: 100%;
            display: flex;
            justify-content: space-around;
            align-items: center;
            margin-bottom: 7%;
        }
        .temp_circle {
            background-color: blueviolet;            
            border-radius: 50%;
            height: 150px;
            width: 150px;
            font-size: 35px;
            font-weight: 600;
        }
        .on_btn {
            border: black solid;
            border-radius: 10%;
            height: 40px;
            width: 130px;
            background-color: greenyellow;         
            font-size: 15px;
            font-weight: 600;        
        }
        .off_btn {
            border: black solid;
            border-radius: 10%;
            height: 40px;
            width: 130px;
            background-color: red;             
            font-size: 15px;
            font-weight: 600;           
        }
        .manual {
            width: 100%;            
            font-size: 35px;
            font-weight: 600;
        }
        footer {
            flex: 0 0 auto;
            background-color: rgb(61, 63, 59);                  
            height: 40px;
            width: 100%;   

        }
    </style>
    </head>

    <body class="flex_center">
        <header class="flex_center">
            <h1>Climate Control Center</h1>
        </header>

       
        <div class="main-content">
            <div class="circle_block">
                <div class="temp_circle flex_center">
                    24.5 C
                </div>
                <div class="temp_circle flex_center">
                    54.5 %
                </div>
            </div>
            <div>
                <form action="/get" class="temp_form">
                    <div>MAX temperature: </div>
                    <div><input type="text" name="max_temp"></div>
                    <div><input type="submit" value="SET" class="btn"></div>  
                </form><br>
        
                <form action="/get" class="temp_form">
                    <div>MIN temperature: </div>
                    <div><input type="text" name="min_temp"></div>
                    <div><input type="submit" value="SET" class="btn"></div>  
                </form><br>
            </div>
            <div class="manual flex_center">
                <div>Manual control</div>
                <div class="circle_block">
                    <button class="flex_center on_btn">OPEN</button>
                    <button class="flex_center off_btn">CLOSE</button>
                </div>
            </div>
        </div>

        <footer class="flex_center">
            2022
        </footer>
    </body>

</html>)rawliteral";