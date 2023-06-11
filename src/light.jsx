import {React, Component} from 'react';
import './App.css'

class Light extends Component {


  render() {
    return (
        <div className={'light ' + this.props.color}>
        </div>
    );
  }
}


export default Light;