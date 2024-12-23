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
       #footer {
            font-size: 15px;
            text-align: center;
            margin-bottom: 0;
        }

        .wifi_signal_1 {
            margin-left: 2%;
            padding: 0 0 0 26px;
            cursor: pointer;
            display: inline-block;
            height: 24px;
            vertical-align: bottom;
            background-image: url("data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiIHN0YW5kYWxvbmU9Im5vIj8+DQo8c3ZnIHhtbG5zOmRjPSJodHRwOi8vcHVybC5vcmcvZGMvZWxlbWVudHMvMS4xLyIgeG1sbnM6Y2M9Imh0dHA6Ly9jcmVhdGl2ZWNvbW1vbnMub3JnL25zIyIgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIiB4bWxuczpzdmc9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHhtbG5zOnNvZGlwb2RpPSJodHRwOi8vc29kaXBvZGkuc291cmNlZm9yZ2UubmV0L0RURC9zb2RpcG9kaS0wLmR0ZCIgeG1sbnM6aW5rc2NhcGU9Imh0dHA6Ly93d3cuaW5rc2NhcGUub3JnL25hbWVzcGFjZXMvaW5rc2NhcGUiIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0IiB2ZXJzaW9uPSIxLjEiIGlkPSJzdmc4NjUiIHNvZGlwb2RpOmRvY25hbWU9IndpZmlfc2lnbmFsXzEuc3ZnIiBpbmtzY2FwZTp2ZXJzaW9uPSIwLjkyLjMgKDI0MDU1NDYsIDIwMTgtMDMtMTEpIj4NCiAgPG1ldGFkYXRhIGlkPSJtZXRhZGF0YTg3MSI+DQogICAgPHJkZjpSREY+DQogICAgICA8Y2M6V29yayByZGY6YWJvdXQ9IiI+DQogICAgICAgIDxkYzpmb3JtYXQ+aW1hZ2Uvc3ZnK3htbDwvZGM6Zm9ybWF0Pg0KICAgICAgICA8ZGM6dHlwZSByZGY6cmVzb3VyY2U9Imh0dHA6Ly9wdXJsLm9yZy9kYy9kY21pdHlwZS9TdGlsbEltYWdlIi8+DQogICAgICAgIDxkYzp0aXRsZS8+DQogICAgICA8L2NjOldvcms+DQogICAgPC9yZGY6UkRGPg0KICA8L21ldGFkYXRhPg0KICA8ZGVmcyBpZD0iZGVmczg2OSIvPg0KICA8c29kaXBvZGk6bmFtZWR2aWV3IHBhZ2Vjb2xvcj0iI2ZmZmZmZiIgYm9yZGVyY29sb3I9IiM2NjY2NjYiIGJvcmRlcm9wYWNpdHk9IjEiIG9iamVjdHRvbGVyYW5jZT0iMTAiIGdyaWR0b2xlcmFuY2U9IjEwIiBndWlkZXRvbGVyYW5jZT0iMTAiIGlua3NjYXBlOnBhZ2VvcGFjaXR5PSIwIiBpbmtzY2FwZTpwYWdlc2hhZG93PSIyIiBpbmtzY2FwZTp3aW5kb3ctd2lkdGg9IjI1NjAiIGlua3NjYXBlOndpbmRvdy1oZWlnaHQ9IjEzMjAiIGlkPSJuYW1lZHZpZXc4NjciIHNob3dncmlkPSJmYWxzZSIgaW5rc2NhcGU6em9vbT0iNDcuNDU4MzMzIiBpbmtzY2FwZTpjeD0iMTIiIGlua3NjYXBlOmN5PSIxMiIgaW5rc2NhcGU6d2luZG93LXg9IjI1NjAiIGlua3NjYXBlOndpbmRvdy15PSIyNyIgaW5rc2NhcGU6d2luZG93LW1heGltaXplZD0iMSIgaW5rc2NhcGU6Y3VycmVudC1sYXllcj0ic3ZnODY1Ii8+DQogIDxwYXRoIGQ9Ik0gNC41MDQsMTMuNTcyIDIuOTk5LDEyLjA4MyBDIDUuMiw5LjI3OCA4LjQxMiw3LjUgMTIsNy41IGMgMy41ODgsMCA2LjgsMS43NzkgOSw0LjU4MyBsIC0xLjUwNCwxLjQ4OSBDIDE3LjY2MSwxMS4yMzQgMTQuOTg0LDkuNzUgMTIsOS43NSBjIC0yLjk4NCwwIC01LjY2MSwxLjQ4NCAtNy40OTYsMy44MjIgeiIgaWQ9InBhdGg4OTgiIHN0eWxlPSJmaWxsOiNjY2NjY2MiLz4NCiAgPHBhdGggZD0ibSAxMiwxNC4yNSBjIDEuNzkxLDAgMy4zOTcsMC44OTEgNC40OTgsMi4yOTMgTCAxOCwxNS4wNTUgQyAxNi41MzMsMTMuMTg2IDE0LjM5MiwxMiAxMiwxMiA5LjYwOCwxMiA3LjQ2NywxMy4xODYgNiwxNS4wNTUgbCAxLjUwMiwxLjQ4OCBDIDguNjAzLDE1LjE0MSAxMC4yMDksMTQuMjUgMTIsMTQuMjUgWiIgaWQ9InBhdGg4OTYiIHN0eWxlPSJmaWxsOiNjY2NjY2MiLz4NCiAgPHBhdGggZD0ibSAxMiwxNi41IGMgLTEuMTk2LDAgLTIuMjU4LDAuNjAyIC0yLjk5LDEuNTM2IEwgMTIsMjEgMTQuOTksMTguMDM3IEMgMTQuMjU4LDE3LjEwMiAxMy4xOTYsMTYuNSAxMiwxNi41IFoiIGlkPSJwYXRoODk0IiBzdHlsZT0iZmlsbDojY2NjY2NjIi8+DQogIDxwYXRoIGQ9Im0gMTIsNS4yNSBjIDQuMTc5LDAgNy45MjcsMi4wNzggMTAuNDk1LDUuMzUxIEwgMjQsOS4xMSBDIDIxLjA2NSw1LjM3MSAxNi43ODMsMyAxMiwzIDcuMjE3LDMgMi45MzUsNS4zNzEgMCw5LjExIGwgMS41MDUsMS40OTEgQyA0LjA3Myw3LjMyOCA3LjgyMSw1LjI1IDEyLDUuMjUgWiIgaWQ9InBhdGg4NjMiIHN0eWxlPSJmaWxsOiNjY2NjY2MiLz4NCjwvc3ZnPg==");
            background-position: 0px 0px;
        }

        .wifi_signal_2 {
            margin-left: 2%;
            padding: 0 0 0 26px;
            cursor: pointer;
            display: inline-block;
            height: 24px;
            vertical-align: bottom;
            background-image: url("data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiIHN0YW5kYWxvbmU9Im5vIj8+DQo8c3ZnIHhtbG5zOmRjPSJodHRwOi8vcHVybC5vcmcvZGMvZWxlbWVudHMvMS4xLyIgeG1sbnM6Y2M9Imh0dHA6Ly9jcmVhdGl2ZWNvbW1vbnMub3JnL25zIyIgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIiB4bWxuczpzdmc9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHhtbG5zOnNvZGlwb2RpPSJodHRwOi8vc29kaXBvZGkuc291cmNlZm9yZ2UubmV0L0RURC9zb2RpcG9kaS0wLmR0ZCIgeG1sbnM6aW5rc2NhcGU9Imh0dHA6Ly93d3cuaW5rc2NhcGUub3JnL25hbWVzcGFjZXMvaW5rc2NhcGUiIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0IiB2ZXJzaW9uPSIxLjEiIGlkPSJzdmc4NjUiIHNvZGlwb2RpOmRvY25hbWU9IndpZmlfc2lnbmFsXzIuc3ZnIiBpbmtzY2FwZTp2ZXJzaW9uPSIwLjkyLjMgKDI0MDU1NDYsIDIwMTgtMDMtMTEpIj4NCiAgPG1ldGFkYXRhIGlkPSJtZXRhZGF0YTg3MSI+DQogICAgPHJkZjpSREY+DQogICAgICA8Y2M6V29yayByZGY6YWJvdXQ9IiI+DQogICAgICAgIDxkYzpmb3JtYXQ+aW1hZ2Uvc3ZnK3htbDwvZGM6Zm9ybWF0Pg0KICAgICAgICA8ZGM6dHlwZSByZGY6cmVzb3VyY2U9Imh0dHA6Ly9wdXJsLm9yZy9kYy9kY21pdHlwZS9TdGlsbEltYWdlIi8+DQogICAgICAgIDxkYzp0aXRsZS8+DQogICAgICA8L2NjOldvcms+DQogICAgPC9yZGY6UkRGPg0KICA8L21ldGFkYXRhPg0KICA8ZGVmcyBpZD0iZGVmczg2OSIvPg0KICA8c29kaXBvZGk6bmFtZWR2aWV3IHBhZ2Vjb2xvcj0iI2ZmZmZmZiIgYm9yZGVyY29sb3I9IiM2NjY2NjYiIGJvcmRlcm9wYWNpdHk9IjEiIG9iamVjdHRvbGVyYW5jZT0iMTAiIGdyaWR0b2xlcmFuY2U9IjEwIiBndWlkZXRvbGVyYW5jZT0iMTAiIGlua3NjYXBlOnBhZ2VvcGFjaXR5PSIwIiBpbmtzY2FwZTpwYWdlc2hhZG93PSIyIiBpbmtzY2FwZTp3aW5kb3ctd2lkdGg9IjI1NjAiIGlua3NjYXBlOndpbmRvdy1oZWlnaHQ9IjEzMjAiIGlkPSJuYW1lZHZpZXc4NjciIHNob3dncmlkPSJmYWxzZSIgaW5rc2NhcGU6em9vbT0iNDcuNDU4MzMzIiBpbmtzY2FwZTpjeD0iMTIiIGlua3NjYXBlOmN5PSIxMiIgaW5rc2NhcGU6d2luZG93LXg9IjI1NjAiIGlua3NjYXBlOndpbmRvdy15PSIyNyIgaW5rc2NhcGU6d2luZG93LW1heGltaXplZD0iMSIgaW5rc2NhcGU6Y3VycmVudC1sYXllcj0ic3ZnODY1Ii8+DQogIDxwYXRoIGQ9Ik0gNC41MDQsMTMuNTcyIDIuOTk5LDEyLjA4MyBDIDUuMiw5LjI3OCA4LjQxMiw3LjUgMTIsNy41IGMgMy41ODgsMCA2LjgsMS43NzkgOSw0LjU4MyBsIC0xLjUwNCwxLjQ4OSBDIDE3LjY2MSwxMS4yMzQgMTQuOTg0LDkuNzUgMTIsOS43NSBjIC0yLjk4NCwwIC01LjY2MSwxLjQ4NCAtNy40OTYsMy44MjIgeiIgaWQ9InBhdGg4OTgiIHN0eWxlPSJmaWxsOiNjY2NjY2MiLz4NCiAgPHBhdGggZD0ibSAxMiwxNC4yNSBjIDEuNzkxLDAgMy4zOTcsMC44OTEgNC40OTgsMi4yOTMgTCAxOCwxNS4wNTUgQyAxNi41MzMsMTMuMTg2IDE0LjM5MiwxMiAxMiwxMiA5LjYwOCwxMiA3LjQ2NywxMy4xODYgNiwxNS4wNTUgbCAxLjUwMiwxLjQ4OCBDIDguNjAzLDE1LjE0MSAxMC4yMDksMTQuMjUgMTIsMTQuMjUgWiIgaWQ9InBhdGg4OTYiIHN0eWxlPSJmaWxsOiNjY2NjY2MiLz4NCiAgPHBhdGggZD0ibSAxMiwxNi41IGMgLTEuMTk2LDAgLTIuMjU4LDAuNjAyIC0yLjk5LDEuNTM2IEwgMTIsMjEgMTQuOTksMTguMDM3IEMgMTQuMjU4LDE3LjEwMiAxMy4xOTYsMTYuNSAxMiwxNi41IFoiIGlkPSJwYXRoODk0Ii8+DQogIDxwYXRoIGQ9Im0gMTIsNS4yNSBjIDQuMTc5LDAgNy45MjcsMi4wNzggMTAuNDk1LDUuMzUxIEwgMjQsOS4xMSBDIDIxLjA2NSw1LjM3MSAxNi43ODMsMyAxMiwzIDcuMjE3LDMgMi45MzUsNS4zNzEgMCw5LjExIGwgMS41MDUsMS40OTEgQyA0LjA3Myw3LjMyOCA3LjgyMSw1LjI1IDEyLDUuMjUgWiIgaWQ9InBhdGg4NjMiIHN0eWxlPSJmaWxsOiNjY2NjY2MiLz4NCjwvc3ZnPg==");
            background-position: 0px 0px;
        }

        .wifi_signal_3 {
            margin-left: 2%;
            padding: 0 0 0 26px;
            cursor: pointer;
            display: inline-block;
            height: 24px;
            vertical-align: bottom;
            background-image: url("data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiIHN0YW5kYWxvbmU9Im5vIj8+DQo8c3ZnIHhtbG5zOmRjPSJodHRwOi8vcHVybC5vcmcvZGMvZWxlbWVudHMvMS4xLyIgeG1sbnM6Y2M9Imh0dHA6Ly9jcmVhdGl2ZWNvbW1vbnMub3JnL25zIyIgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIiB4bWxuczpzdmc9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHhtbG5zOnNvZGlwb2RpPSJodHRwOi8vc29kaXBvZGkuc291cmNlZm9yZ2UubmV0L0RURC9zb2RpcG9kaS0wLmR0ZCIgeG1sbnM6aW5rc2NhcGU9Imh0dHA6Ly93d3cuaW5rc2NhcGUub3JnL25hbWVzcGFjZXMvaW5rc2NhcGUiIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0IiB2ZXJzaW9uPSIxLjEiIGlkPSJzdmc4NjUiIHNvZGlwb2RpOmRvY25hbWU9IndpZmlfc2lnbmFsXzMuc3ZnIiBpbmtzY2FwZTp2ZXJzaW9uPSIwLjkyLjMgKDI0MDU1NDYsIDIwMTgtMDMtMTEpIj4NCiAgPG1ldGFkYXRhIGlkPSJtZXRhZGF0YTg3MSI+DQogICAgPHJkZjpSREY+DQogICAgICA8Y2M6V29yayByZGY6YWJvdXQ9IiI+DQogICAgICAgIDxkYzpmb3JtYXQ+aW1hZ2Uvc3ZnK3htbDwvZGM6Zm9ybWF0Pg0KICAgICAgICA8ZGM6dHlwZSByZGY6cmVzb3VyY2U9Imh0dHA6Ly9wdXJsLm9yZy9kYy9kY21pdHlwZS9TdGlsbEltYWdlIi8+DQogICAgICAgIDxkYzp0aXRsZS8+DQogICAgICA8L2NjOldvcms+DQogICAgPC9yZGY6UkRGPg0KICA8L21ldGFkYXRhPg0KICA8ZGVmcyBpZD0iZGVmczg2OSIvPg0KICA8c29kaXBvZGk6bmFtZWR2aWV3IHBhZ2Vjb2xvcj0iI2ZmZmZmZiIgYm9yZGVyY29sb3I9IiM2NjY2NjYiIGJvcmRlcm9wYWNpdHk9IjEiIG9iamVjdHRvbGVyYW5jZT0iMTAiIGdyaWR0b2xlcmFuY2U9IjEwIiBndWlkZXRvbGVyYW5jZT0iMTAiIGlua3NjYXBlOnBhZ2VvcGFjaXR5PSIwIiBpbmtzY2FwZTpwYWdlc2hhZG93PSIyIiBpbmtzY2FwZTp3aW5kb3ctd2lkdGg9IjI1NjAiIGlua3NjYXBlOndpbmRvdy1oZWlnaHQ9IjEzMjAiIGlkPSJuYW1lZHZpZXc4NjciIHNob3dncmlkPSJmYWxzZSIgaW5rc2NhcGU6em9vbT0iNDcuNDU4MzMzIiBpbmtzY2FwZTpjeD0iMTIiIGlua3NjYXBlOmN5PSIxMiIgaW5rc2NhcGU6d2luZG93LXg9IjI1NjAiIGlua3NjYXBlOndpbmRvdy15PSIyNyIgaW5rc2NhcGU6d2luZG93LW1heGltaXplZD0iMSIgaW5rc2NhcGU6Y3VycmVudC1sYXllcj0ic3ZnODY1Ii8+DQogIDxwYXRoIGQ9Ik0gNC41MDQsMTMuNTcyIDIuOTk5LDEyLjA4MyBDIDUuMiw5LjI3OCA4LjQxMiw3LjUgMTIsNy41IGMgMy41ODgsMCA2LjgsMS43NzkgOSw0LjU4MyBsIC0xLjUwNCwxLjQ4OSBDIDE3LjY2MSwxMS4yMzQgMTQuOTg0LDkuNzUgMTIsOS43NSBjIC0yLjk4NCwwIC01LjY2MSwxLjQ4NCAtNy40OTYsMy44MjIgeiIgaWQ9InBhdGg4OTgiIHN0eWxlPSJmaWxsOiNjY2NjY2MiLz4NCiAgPHBhdGggZD0ibSAxMiwxNC4yNSBjIDEuNzkxLDAgMy4zOTcsMC44OTEgNC40OTgsMi4yOTMgTCAxOCwxNS4wNTUgQyAxNi41MzMsMTMuMTg2IDE0LjM5MiwxMiAxMiwxMiA5LjYwOCwxMiA3LjQ2NywxMy4xODYgNiwxNS4wNTUgbCAxLjUwMiwxLjQ4OCBDIDguNjAzLDE1LjE0MSAxMC4yMDksMTQuMjUgMTIsMTQuMjUgWiIgaWQ9InBhdGg4OTYiLz4NCiAgPHBhdGggZD0ibSAxMiwxNi41IGMgLTEuMTk2LDAgLTIuMjU4LDAuNjAyIC0yLjk5LDEuNTM2IEwgMTIsMjEgMTQuOTksMTguMDM3IEMgMTQuMjU4LDE3LjEwMiAxMy4xOTYsMTYuNSAxMiwxNi41IFoiIGlkPSJwYXRoODk0Ii8+DQogIDxwYXRoIGQ9Im0gMTIsNS4yNSBjIDQuMTc5LDAgNy45MjcsMi4wNzggMTAuNDk1LDUuMzUxIEwgMjQsOS4xMSBDIDIxLjA2NSw1LjM3MSAxNi43ODMsMyAxMiwzIDcuMjE3LDMgMi45MzUsNS4zNzEgMCw5LjExIGwgMS41MDUsMS40OTEgQyA0LjA3Myw3LjMyOCA3LjgyMSw1LjI1IDEyLDUuMjUgWiIgaWQ9InBhdGg4NjMiIHN0eWxlPSJmaWxsOiNjY2NjY2MiLz4NCjwvc3ZnPg==");
            background-position: 0px 0px;
        }

        .wifi_signal_4 {
            margin-left: 2%;
            padding: 0 0 0 26px;
            cursor: pointer;
            display: inline-block;
            height: 24px;
            vertical-align: bottom;
            background-image: url("data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiIHN0YW5kYWxvbmU9Im5vIj8+DQo8c3ZnIHhtbG5zOmRjPSJodHRwOi8vcHVybC5vcmcvZGMvZWxlbWVudHMvMS4xLyIgeG1sbnM6Y2M9Imh0dHA6Ly9jcmVhdGl2ZWNvbW1vbnMub3JnL25zIyIgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIiB4bWxuczpzdmc9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHhtbG5zOnNvZGlwb2RpPSJodHRwOi8vc29kaXBvZGkuc291cmNlZm9yZ2UubmV0L0RURC9zb2RpcG9kaS0wLmR0ZCIgeG1sbnM6aW5rc2NhcGU9Imh0dHA6Ly93d3cuaW5rc2NhcGUub3JnL25hbWVzcGFjZXMvaW5rc2NhcGUiIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0IiB2ZXJzaW9uPSIxLjEiIGlkPSJzdmc4NjUiIHNvZGlwb2RpOmRvY25hbWU9IndpZmlfc2lnbmFsXzQuc3ZnIiBpbmtzY2FwZTp2ZXJzaW9uPSIwLjkyLjMgKDI0MDU1NDYsIDIwMTgtMDMtMTEpIj4NCiAgPG1ldGFkYXRhIGlkPSJtZXRhZGF0YTg3MSI+DQogICAgPHJkZjpSREY+DQogICAgICA8Y2M6V29yayByZGY6YWJvdXQ9IiI+DQogICAgICAgIDxkYzpmb3JtYXQ+aW1hZ2Uvc3ZnK3htbDwvZGM6Zm9ybWF0Pg0KICAgICAgICA8ZGM6dHlwZSByZGY6cmVzb3VyY2U9Imh0dHA6Ly9wdXJsLm9yZy9kYy9kY21pdHlwZS9TdGlsbEltYWdlIi8+DQogICAgICAgIDxkYzp0aXRsZS8+DQogICAgICA8L2NjOldvcms+DQogICAgPC9yZGY6UkRGPg0KICA8L21ldGFkYXRhPg0KICA8ZGVmcyBpZD0iZGVmczg2OSIvPg0KICA8c29kaXBvZGk6bmFtZWR2aWV3IHBhZ2Vjb2xvcj0iI2ZmZmZmZiIgYm9yZGVyY29sb3I9IiM2NjY2NjYiIGJvcmRlcm9wYWNpdHk9IjEiIG9iamVjdHRvbGVyYW5jZT0iMTAiIGdyaWR0b2xlcmFuY2U9IjEwIiBndWlkZXRvbGVyYW5jZT0iMTAiIGlua3NjYXBlOnBhZ2VvcGFjaXR5PSIwIiBpbmtzY2FwZTpwYWdlc2hhZG93PSIyIiBpbmtzY2FwZTp3aW5kb3ctd2lkdGg9IjI1NjAiIGlua3NjYXBlOndpbmRvdy1oZWlnaHQ9IjEzMjAiIGlkPSJuYW1lZHZpZXc4NjciIHNob3dncmlkPSJmYWxzZSIgaW5rc2NhcGU6em9vbT0iNDcuNDU4MzMzIiBpbmtzY2FwZTpjeD0iMTIiIGlua3NjYXBlOmN5PSIxMiIgaW5rc2NhcGU6d2luZG93LXg9IjI1NjAiIGlua3NjYXBlOndpbmRvdy15PSIyNyIgaW5rc2NhcGU6d2luZG93LW1heGltaXplZD0iMSIgaW5rc2NhcGU6Y3VycmVudC1sYXllcj0ic3ZnODY1Ii8+DQogIDxwYXRoIGQ9Ik0gNC41MDQsMTMuNTcyIDIuOTk5LDEyLjA4MyBDIDUuMiw5LjI3OCA4LjQxMiw3LjUgMTIsNy41IGMgMy41ODgsMCA2LjgsMS43NzkgOSw0LjU4MyBsIC0xLjUwNCwxLjQ4OSBDIDE3LjY2MSwxMS4yMzQgMTQuOTg0LDkuNzUgMTIsOS43NSBjIC0yLjk4NCwwIC01LjY2MSwxLjQ4NCAtNy40OTYsMy44MjIgeiIgaWQ9InBhdGg4OTgiLz4NCiAgPHBhdGggZD0ibSAxMiwxNC4yNSBjIDEuNzkxLDAgMy4zOTcsMC44OTEgNC40OTgsMi4yOTMgTCAxOCwxNS4wNTUgQyAxNi41MzMsMTMuMTg2IDE0LjM5MiwxMiAxMiwxMiA5LjYwOCwxMiA3LjQ2NywxMy4xODYgNiwxNS4wNTUgbCAxLjUwMiwxLjQ4OCBDIDguNjAzLDE1LjE0MSAxMC4yMDksMTQuMjUgMTIsMTQuMjUgWiIgaWQ9InBhdGg4OTYiLz4NCiAgPHBhdGggZD0ibSAxMiwxNi41IGMgLTEuMTk2LDAgLTIuMjU4LDAuNjAyIC0yLjk5LDEuNTM2IEwgMTIsMjEgMTQuOTksMTguMDM3IEMgMTQuMjU4LDE3LjEwMiAxMy4xOTYsMTYuNSAxMiwxNi41IFoiIGlkPSJwYXRoODk0Ii8+DQogIDxwYXRoIGQ9Im0gMTIsNS4yNSBjIDQuMTc5LDAgNy45MjcsMi4wNzggMTAuNDk1LDUuMzUxIEwgMjQsOS4xMSBDIDIxLjA2NSw1LjM3MSAxNi43ODMsMyAxMiwzIDcuMjE3LDMgMi45MzUsNS4zNzEgMCw5LjExIGwgMS41MDUsMS40OTEgQyA0LjA3Myw3LjMyOCA3LjgyMSw1LjI1IDEyLDUuMjUgWiIgaWQ9InBhdGg4NjMiIHN0eWxlPSJmaWxsOiNjY2NjY2MiLz4NCjwvc3ZnPg==");
            background-position: 0px 0px;
        }

        .wifi_signal_5 {
            margin-left: 2%;
            padding: 0 0 0 26px;
            cursor: pointer;
            display: inline-block;
            height: 24px;
            vertical-align: bottom;
            background-image: url("data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiIHN0YW5kYWxvbmU9Im5vIj8+DQo8c3ZnIHhtbG5zOmRjPSJodHRwOi8vcHVybC5vcmcvZGMvZWxlbWVudHMvMS4xLyIgeG1sbnM6Y2M9Imh0dHA6Ly9jcmVhdGl2ZWNvbW1vbnMub3JnL25zIyIgeG1sbnM6cmRmPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIiB4bWxuczpzdmc9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHhtbG5zOnNvZGlwb2RpPSJodHRwOi8vc29kaXBvZGkuc291cmNlZm9yZ2UubmV0L0RURC9zb2RpcG9kaS0wLmR0ZCIgeG1sbnM6aW5rc2NhcGU9Imh0dHA6Ly93d3cuaW5rc2NhcGUub3JnL25hbWVzcGFjZXMvaW5rc2NhcGUiIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0IiB2ZXJzaW9uPSIxLjEiIGlkPSJzdmc4NjUiIHNvZGlwb2RpOmRvY25hbWU9IndpZmlfc2lnbmFsXzUuc3ZnIiBpbmtzY2FwZTp2ZXJzaW9uPSIwLjkyLjMgKDI0MDU1NDYsIDIwMTgtMDMtMTEpIj4NCiAgPG1ldGFkYXRhIGlkPSJtZXRhZGF0YTg3MSI+DQogICAgPHJkZjpSREY+DQogICAgICA8Y2M6V29yayByZGY6YWJvdXQ9IiI+DQogICAgICAgIDxkYzpmb3JtYXQ+aW1hZ2Uvc3ZnK3htbDwvZGM6Zm9ybWF0Pg0KICAgICAgICA8ZGM6dHlwZSByZGY6cmVzb3VyY2U9Imh0dHA6Ly9wdXJsLm9yZy9kYy9kY21pdHlwZS9TdGlsbEltYWdlIi8+DQogICAgICAgIDxkYzp0aXRsZS8+DQogICAgICA8L2NjOldvcms+DQogICAgPC9yZGY6UkRGPg0KICA8L21ldGFkYXRhPg0KICA8ZGVmcyBpZD0iZGVmczg2OSIvPg0KICA8c29kaXBvZGk6bmFtZWR2aWV3IHBhZ2Vjb2xvcj0iI2ZmZmZmZiIgYm9yZGVyY29sb3I9IiM2NjY2NjYiIGJvcmRlcm9wYWNpdHk9IjEiIG9iamVjdHRvbGVyYW5jZT0iMTAiIGdyaWR0b2xlcmFuY2U9IjEwIiBndWlkZXRvbGVyYW5jZT0iMTAiIGlua3NjYXBlOnBhZ2VvcGFjaXR5PSIwIiBpbmtzY2FwZTpwYWdlc2hhZG93PSIyIiBpbmtzY2FwZTp3aW5kb3ctd2lkdGg9IjI1NjAiIGlua3NjYXBlOndpbmRvdy1oZWlnaHQ9IjEzMjAiIGlkPSJuYW1lZHZpZXc4NjciIHNob3dncmlkPSJmYWxzZSIgaW5rc2NhcGU6em9vbT0iNDcuNDU4MzMzIiBpbmtzY2FwZTpjeD0iMTIiIGlua3NjYXBlOmN5PSIxMiIgaW5rc2NhcGU6d2luZG93LXg9IjI1NjAiIGlua3NjYXBlOndpbmRvdy15PSIyNyIgaW5rc2NhcGU6d2luZG93LW1heGltaXplZD0iMSIgaW5rc2NhcGU6Y3VycmVudC1sYXllcj0ic3ZnODY1Ii8+DQogIDxwYXRoIGQ9Ik00LjUwNCAxMy41NzJsLTEuNTA1LTEuNDg5YzIuMjAxLTIuODA1IDUuNDEzLTQuNTgzIDkuMDAxLTQuNTgzczYuOCAxLjc3OSA5IDQuNTgzbC0xLjUwNCAxLjQ4OWMtMS44MzUtMi4zMzgtNC41MTItMy44MjItNy40OTYtMy44MjJzLTUuNjYxIDEuNDg0LTcuNDk2IDMuODIyem03LjQ5Ni42NzhjMS43OTEgMCAzLjM5Ny44OTEgNC40OTggMi4yOTNsMS41MDItMS40ODhjLTEuNDY3LTEuODY5LTMuNjA4LTMuMDU1LTYtMy4wNTVzLTQuNTMzIDEuMTg2LTYgMy4wNTVsMS41MDIgMS40ODhjMS4xMDEtMS40MDIgMi43MDctMi4yOTMgNC40OTgtMi4yOTN6bTAgMi4yNWMtMS4xOTYgMC0yLjI1OC42MDItMi45OSAxLjUzNmwyLjk5IDIuOTY0IDIuOTktMi45NjNjLS43MzItLjkzNS0xLjc5NC0xLjUzNy0yLjk5LTEuNTM3em0wLTExLjI1YzQuMTc5IDAgNy45MjcgMi4wNzggMTAuNDk1IDUuMzUxbDEuNTA1LTEuNDkxYy0yLjkzNS0zLjczOS03LjIxNy02LjExLTEyLTYuMTFzLTkuMDY1IDIuMzcxLTEyIDYuMTFsMS41MDUgMS40OTFjMi41NjgtMy4yNzMgNi4zMTYtNS4zNTEgMTAuNDk1LTUuMzUxeiIgaWQ9InBhdGg4NjMiLz4NCjwvc3ZnPg==");
            background-position: 0px 0px;
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
                                <th>STA Network</th>
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
                                <td>PSK:</td>
                                <td colspan="2" id="PSK"></td>
                            </tr>
                        </table>
                        <button id="wifiChange">Change WiFi network</button>
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
                    </div>
                    <div id="ap_view" style="display: none">
                        <table>
                            <tr>
                                <th>AP Network</th>
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

                                    scanImg.setAttribute('class', 'wifi_signal_' + strength(scanResults[i].rssi));
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
                                d.callback(x.status, x.responseText);
                                if (that.queue.length === 0) that.running = false;
                                if (that.running) that._request(that.queue.shift());
                            }
                        }
                    }

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
                const btn_wifiSave = ge("wifiSave");
                const btn_wifiChange = ge("wifiChange");

                const scanList = ge("scanList");

                btn_wifiChange.onclick = function (e) {
                    ge('wired_view').style.display = "none";
                    ge('sta_view').style.display = "none";
                    ge('ap_view').style.display = "none";
                    ge('ap_scan_view').style.display = "block";
                    run_WiFi_Scan();
                }

                function httpPostSaveNetwork(status, responseText) {
                    if (status != 200) {
                        alert("ERROR[" + status + "]: " + responseText);
                    } else {
                        btn_wifiSave.innerText = "Saved";
                    }
                }
                                
                btn_wifiSave.onclick = function (e) {
                    btn_wifiSave.innerText = 'Saving';
                    if (sta_ssid === "") {
                        alert("Please select network");
                    } else {
                        btn_wifiSave.innerText = "Saving";
                        var formData = new FormData();
                        formData.append("ssid", sta_ssid.value);
                        formData.append("pass", sta_pass.value);
                        requests.add("POST", endpoint + "/savenetwork", formData, httpPostSaveNetwork);
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