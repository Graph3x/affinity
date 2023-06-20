import {React, Component} from 'react';
import { Line } from "react-chartjs-2";
import Chart from 'chart.js/auto';


class LineChart extends Component {

    state = {
        labels: this.props.labels,
                 
                 
        options: {
            maintainAspectRatio: false,
        },

        datasets: [
          {
            label: 'Sonda',
            fill: false,
            lineTension: 0.3,
            backgroundColor: 'rgba(75,192,192,1)',
            borderColor: 'rgba(100,50,255,1)',
            borderWidth: 3,
            data: this.props.data,
          }
        ]
      }


      render() {
        return (
          <div id='graph'>
            {console.log(this.props)}
            <Line
              data={this.state}
              options={{
                title:{
                  display:false,
                  text:'Average Rainfall per month',
                  fontSize:20
                },
                legend:{
                  display:false,
                  position:'right'
                }
              }}
            />
          </div>
        );
      }
}


export default LineChart;