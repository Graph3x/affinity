from fastapi import FastAPI, Request
import json

app = FastAPI()


# INTER ENDPOINT GLOBALS
trac_data = {
    "gps": "NaN",
    "speed": "NaN",
    "padak": "NaN",
    "magnet": "NaN",
    "sirena": "NaN",
}

Launched = False


def struct_module(data: list) -> dict:

    op_code = 1
    global Launched
    if not Launched:

        if trac_data["magnet"] == "1":
            op_code = 2
        elif trac_data["magnet"] != "0":
            op_code = 3
            Launched = True
    else:
        op_code = 4
        if trac_data["padak"] != "0":
            op_code = 5

        if float(data[2]) < 3:
            op_code = 6

    return{
        "time": data[0],
        "gps": data[1],
        "speed": data[2],
        "height": data[3],
        "acc_x": data[4],
        "acc_y": data[5],
        "acc_z": data[6],
        "temp": data[7],
        "pressure": data[8],
        "humid": data[9],
        "op_code": 1,
        "mag": trac_data["magnet"],
        "gp2": trac_data["gps"],
        "padak": trac_data["padak"],
        "sirena": trac_data["sirena"]
    }


@app.post("/")
async def posted(request: Request):

    sus_data = request.headers['content-type']
    data = struct_module(sus_data.split("e"))

    dat = json.dumps(data)
    print(dat)

    with open("data.cj", "a") as f:
        f.write(dat)
        f.write("\n")

    return "OK"


@app.post("/tracker")
async def posted_tracker(request: Request):

    sus_data = request.headers['content-type']

    sdata = sus_data.split('a')
    global trac_data
    trac_data = {
        "gps": sdata[0],
        "speed": sdata[1],
        "padak": sdata[2],
        "magnet": sdata[3],
        "sirena": sdata[4],
    }

    print(json.dumps(trac_data))

    return "OK"


@app.get("/")
async def root():
    return "OK"
