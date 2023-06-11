import {React, Component} from 'react';
import './App.css'

class Displayer extends Component {


  render() {
    return (
        <div className='displayer' id={this.props.uname}>
            <h3>{this.props.value}</h3>
        </div>
    );
  }
}


export default Displayer;