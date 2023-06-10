import {React, Component} from 'react';
import './App.css'
import Displayer from './displayer';

class Homepage extends Component {
   constructor(props) {
    super(props);
    this.state = {
      data: [],
    };
  }

  componentDidMount() {
    console.log("mount")
    const socket = new WebSocket('ws://localhost:8080');

    socket.onmessage = (event) => {
      const newData = JSON.parse(event.data);
      console.log(newData)
      this.setState((prevState) => ({
        data: [...prevState.data, newData],
      }));
    };

    socket.onclose = () => {
      console.log('WebSocket connection closed');
    };
  }


  render() {

    let test = "000.00"

    if(this.state.data)
    {
      if(this.state.data[this.state.data.length - 1]){
        test = this.state.data[this.state.data.length - 1].test
      }
    }

    return (
        <div id='root'>
            <div id='body-div'>
              <div id='test-disp'>
                <Displayer value={test}/>
              </div>
              
            </div>
        </div>
    );
  }
}


export default Homepage;