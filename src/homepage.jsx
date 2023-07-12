import {React, Component} from 'react';
import './App.css'
import Displayer from './displayer';
import Light from './light';
import LineChart from './LineGraph';

const OFFSET = 527;

class Homepage extends Component {
   constructor(props) {
    super(props);
    this.state = {
      data: [],
      seconds: -0.750
    };
  }

  
  componentDidMount() {
    const socket = new WebSocket('ws://109.80.17.78:40271');

    socket.onmessage = (event) => {
      const newData = JSON.parse(event.data);
      console.log(newData)
      this.setState((prevState) => ({
        data: [...prevState.data, newData],
      }));
    };

    socket.onclose = () => {
      if (this.state.data) {
        if(this.state.data[this.state.data.length -1]){
        let newData = this.state.data[this.state.data.length -1];
        newData.op_code = 0;
        this.setState((prevState) => ({
        data: [...prevState.data, newData],
      }));
    }};
      clearInterval(this.timerID);
      console.log('WebSocket connection closed');
    }
  }


  tick()
  {
    this.setState((prevState) => ({
      seconds: prevState.seconds + 0.125,
    }));
  }

  render() {

    let op_code = 0;

    let speed = "000.00";
    let height = "000.00";
    let temp = "0";
    let pressure = "0";
    let status = "OFFLINE";
    let time = "00:00";
    let padak = "red";
    let sirena = "red";
    let magnet = "red";

    let labels = []
    let data_rend = []

    let display = []

    if(this.state.data)
    {
      if(this.state.data[this.state.data.length - 1]){

        labels = this.state.data.map(a => Math.floor(parseInt(a.time) / 1000).toString())

        data_rend = this.state.data.map(a => parseFloat((a.height - OFFSET).toFixed(2)))

        speed = this.state.data[this.state.data.length - 1].speed
        height = (this.state.data[this.state.data.length - 1].height - OFFSET).toFixed(2)
        temp = this.state.data[this.state.data.length - 1].temp
        pressure = this.state.data[this.state.data.length - 1].pressure
        op_code = this.state.data[this.state.data.length - 1].op_code
        time =  `${Math.floor(Math.max(this.state.seconds, 0) / 60).toString().padStart(2, '0')}:${(Math.floor(Math.max(this.state.seconds, 0) % 60)).toString().padStart(2, '0')}`;

        if(this.state.data[this.state.data.length - 1].padak != "0" && this.state.data[this.state.data.length - 1].padak != "NaN"){
          padak = "green";
        }

        if(this.state.data[this.state.data.length - 1].mag != "0" && this.state.data[this.state.data.length - 1].mag != "NaN"){
          magnet = "green"
        }

        if(this.state.data[this.state.data.length - 1].sirena != "0" && this.state.data[this.state.data.length - 1].sirena != "NaN"){
          sirena = "green"
        }

        switch(op_code)
        {
          case 0:
            status = "OFFLINE";
            break;
          case 1:
            status = "ONLINE";
            break;
          case 2:
            status = "ARMED";
            break;
          case 3:
            status = "LAUNCH";
            this.timerID = setInterval(() => {this.tick();}, 500);
            break;
          case 4:
            status = "IN AIR";
            break;
          case 5:
            status = "RECOVERY";
            break;
          case 6:
            status = "TOUCHDOWN";
            break;
        }

        if(this.state.data.length > 2){
          display.push(JSON.stringify(this.state.data[this.state.data.length - 3], null, 2))
        }

        if(this.state.data.length > 1){
          display.push(JSON.stringify(this.state.data[this.state.data.length - 2], null, 2))
        }

        display.push(JSON.stringify(this.state.data[this.state.data.length - 1], null, 2))
        
      }
    }

    return (
        <div id='root'>
            <div id='body-div'>

              <div id='status-disp'>
                <Displayer value={status}/>
              </div>

              <div id='speed-disp'>
                <Displayer value={speed + "M/S"}/>
              </div>

              <div id='height-disp'>
                <Displayer value={height + "M"}/>
              </div>

              <div id='temp-disp'>
                <Displayer value={temp + "°C"}/>
              </div>

              <div id='press-disp'>
                <Displayer value={pressure + "HPA"}/>
              </div>

              <div id='time-disp'>
                <Displayer value={time} uname="time-inner"/>
              </div>
              <div id='padak'>
                <Light color={padak}/>
              </div>
              <div id='sirena'>
                <Light color={sirena}/>
              </div>
              <div id='magnet'>
                <Light color={magnet}/>
              </div>
              <div id="chart-div">
                <LineChart labels={labels} data={data_rend}/>
              </div>
              <div id='console'>
                <p>{display[0]}</p>
                <br/>
                <p>{display[1]}</p>
                <br/>
                <p>{display[2]}</p>
              </div>

            </div>
        </div>
    );
  }
}

/* [527.9,527.8,527.9,527.7,527.7,527.7,527.7,526.6,526.6,526.6,526.9,526.8,526.8] 
["599","600","601","602","603","604","828","830","831","832","833","834","834"]
*/

export default Homepage;