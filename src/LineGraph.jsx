import {React, Component} from 'react';
import { Line } from "react-chartjs-2";
import Chart from 'chart.js/auto';

class LineChart extends Component {
  state = {
    labels: [],
    datasets: [
      {
        label: '',
        fill: false,
        lineTension: 0.3,
        backgroundColor: 'rgba(75,192,192,1)',
        borderColor: 'rgba(100,50,255,1)',
        borderWidth: 3,
        pointStyle: "none", 
        data: [],
      },
    ],
    options: {
      maintainAspectRatio: false,
      animation: false,
      title: {
        display: false,
        text: 'Raketa bby',
        fontSize: 20,
      },
      legend: {
        display: false,
      },
    },
  };

  componentDidUpdate(prevProps) {
    if (prevProps.labels !== this.props.labels || prevProps.data !== this.props.data) {
      this.setState({
        labels: this.props.labels,
        datasets: [
          {
            ...this.state.datasets[0],
            data: this.props.data,
          },
        ],
      });
    }
  }

  render() {
    return (
      <div id="graph" key={this.props.labels[0]}>
        <Line redraw={true} data={this.state} options={this.state.options} />
      </div>
    );
  }
}

export default LineChart;