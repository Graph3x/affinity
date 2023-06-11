import {React, Component} from 'react';
import './App.css'
import Displayer from './displayer';
import Light from './light';

class Homepage extends Component {
   constructor(props) {
    super(props);
    this.state = {
      data: [],
      seconds: -1.5,
    };
  }

  
  componentDidMount() {
    const socket = new WebSocket('ws://localhost:8080');

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
      seconds: prevState.seconds + 0.5,
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
    let motor = false;
    let start = false;
    let padak = false;
    let dopad = false;

    if(this.state.data)
    {
      if(this.state.data[this.state.data.length - 1]){
        speed = this.state.data[this.state.data.length - 1].speed
        height = this.state.data[this.state.data.length - 1].height
        temp = this.state.data[this.state.data.length - 1].temp
        pressure = this.state.data[this.state.data.length - 1].pressure
        op_code = this.state.data[this.state.data.length - 1].op_code
        time =  `${Math.floor(Math.max(this.state.seconds, 0) / 60).toString().padStart(2, '0')}:${(Math.max(this.state.seconds, 0) % 60).toString().padStart(2, '0')}`;
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
            this.timerID = setInterval(() => {this.tick();}, 1000);
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
              <div>
                <Light/>
              </div>
              
            </div>
        </div>
    );
  }
}


export default Homepage;