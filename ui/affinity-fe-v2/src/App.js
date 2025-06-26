import './App.css';
import React from "react";
import { useWebSocket } from "./useWebSocket";

function GroundStation() {
  return (
    <div id="ground-station">

    </div>
  )
}

const temporaryDisplay = (msg, i) => {
  let data = msg.split(',');
  let displayable = `time: ${data[1]}, temp: ${parseFloat(data[2]).toFixed(2)}, press: ${parseFloat(data[3]).toFixed(2)}, status: ${data[4]}`;
  return (
    <li key={i}>{displayable}</li>
  )
}

function App() {
  const targetUrl = window.location.search.split("?source=")[1];
  const messages = useWebSocket("ws://" + targetUrl + ":14961");
  return (
    <div className="App">
      <div>
        <h2>AFFINITY v2 - upgrade deluxe:</h2>
        <ul>
          {messages.map((msg, i) => temporaryDisplay(msg, i))}
        </ul>
      </div>
    </div>
  );
}

export default App;
