import microcontroller
import socketpool
import wifi

from adafruit_httpserver import Server, Request, Response, JSONResponse, GET, POST, PUT, DELETE
from adafruit_httpserver import Status


import binascii


def format_mac_address(binary_mac):
    # Convert binary data to a hexadecimal string
    hex_mac = binascii.hexlify(binary_mac).decode("ascii")
    
    # Format with colons
    human_readable_mac = ":".join(hex_mac[i : i + 2] for i in range(0, 12, 2))
    
    return human_readable_mac


def printWifiInfo():
    mac_addr = format_mac_address(wifi.radio.mac_address)
    hostname = wifi.radio.hostname
    ip4_address = wifi.radio.ipv4_address
    
    print("\r\n-------------------------")
    print(f"Mac Address:{mac_addr}")
    print(f"Hostname:{hostname}")
    print(f"IPv4 Address:{ip4_address}")
    print("-------------------------\r\n\r\n")
    


printWifiInfo()

pool = socketpool.SocketPool(wifi.radio)
server = Server(pool, debug=True)

ledModes = ["ON", "OFF"]
ledMode = "OFF"

objects = [
    {"id": 1, "name": "Andrii is a schmeckle","ledModes": ledModes,"currentLEDMode":ledMode},
]

ledObject ={"id": 1, "name": "Andrii is a schmeckle","ledModes": ledModes,"currentLEDMode":ledMode}

#uploaded_object ={"id": 1, "name": "Andrii is a schmeckle","ledModes": ledModes,"currentLEDMode":ledMode}


# (Optional) Allow cross-origin requests.
server.headers = {
    "Access-Control-Allow-Origin": "*",
}

led_modes = ["OFF", "ALL", "EVEN", "ODD", "EVEN/ODD"]


@server.route("/cpu-information", append_slash=True)
def cpu_information_handler(request: Request):
    """
    Return the current CPU temperature, frequency, and voltage as JSON.
    """
    
    data = {
        "temperature": microcontroller.cpu.temperature,
        "frequency": microcontroller.cpu.frequency,
        "voltage": microcontroller.cpu.voltage,
    }

    return JSONResponse(request, data)


@server.route("/api", [GET, POST, PUT, DELETE], append_slash=True)
def api(request: Request):
    """
    Performs different operations depending on the HTTP method.
    """
    
    # Get objects
    if request.method == GET:
        return JSONResponse(request, objects)
    
    # Upload or update objects
    if request.method in [POST, PUT]:
        uploaded_object = request.json()
    
        # Find object with same ID
        for i, obj in enumerate(objects):
            if obj["id"] == uploaded_object["id"]:
                objects[i] = uploaded_object
                
                return JSONResponse(
                    request, {"message": "Object updated", "object": uploaded_object}
                )
                
        # If not found, add it
        objects.append(uploaded_object)
        return JSONResponse(
            request, {"message": "Object added", "object": uploaded_object}
        )
        
    # Delete objects
    if request.method == DELETE:
        deleted_object = request.json()
        
        # Find object with same ID
        for i, obj in enumerate(objects):
            if obj["id"] == deleted_object["id"]:
                del objects[i]
                
                return JSONResponse(
                    request, {"message": "Object deleted", "object": deleted_object}
                )
        
        # If not found, return error
        return JSONResponse(
            request, {"message": "Object not found", "object": deleted_object}
        )
    
    # If we get here, something went wrong
    return JSONResponse(request, {"message": "Something went wrong"})


@server.route("/led-effect", [GET, POST, PUT], append_slash=True)
def api(request: Request):
    ##global ledMode
    """
    Performs different operations depending on the HTTP method.
    """
    
    # Get objects
    if request.method == GET:
        return JSONResponse(request, ledObject)
    
    # Upload or update objects
    if request.method in [POST, PUT]:
        uploaded_object = request.json()
        
        print(f"POST:Got Object:{uploaded_object}")
        
        try:
            #obj_id = 'id' in uploaded_object ? uploaded_object['id'] : None
            obj_id = uploaded_object['id'] if 'id' in uploaded_object and uploaded_object['id'] == 1 else None
            assert obj_id
            
            newLEDMode = uploaded_object['currentLEDMode'] if \
                'currentLEDMode' in uploaded_object and \
                    uploaded_object['currentLEDMode'] in ledModes else None
            assert newLEDMode
            ledObject['currentLEDMode'] = newLEDMode
            ##ledMode = newLEDMode
            
        except Exception as e:
            print("Bad client request")
            return Response(request, status=Status(400, "Bad Request"))
        # Find object with same ID
        # for i, obj in enumerate(objects):
        #     if obj["id"] == uploaded_object["id"]:
        #         objects[i] = uploaded_object
                
        #         return JSONResponse(
        #             request, {"message": "Object updated", "object": uploaded_object}
        #         )
                
        # # If not found, add it
        # objects.append(uploaded_object)
        return Response(request, status=Status(200,"OK"))
    
    # If we get here, something went wrong
    #return JSONResponse(request, {"message": "Something went wrong"})
    return Response(request, status=Status(400, "Bad Request"))



server.serve_forever(str(wifi.radio.ipv4_address))
