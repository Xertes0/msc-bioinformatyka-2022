import "./ProteinInfo.css";
import {
  BarElement,
  CategoryScale,
  Chart as ChartJS,
  Legend,
  LinearScale,
  Title,
  Tooltip,
} from "chart.js";
import zoomPlugin from "chartjs-plugin-zoom";
import { Bar } from "react-chartjs-2";
import { AminoAcidData } from "../AminoAcidData";
import React from "react";

ChartJS.register(
  CategoryScale,
  LinearScale,
  BarElement,
  Title,
  Tooltip,
  Legend,
  zoomPlugin
);
export const options = {
  indexAxis: "y" as const,
  responsive: true,
  maintainAspectRatio: false,
  interaction: {
    mode: "index" as const,
    intersect: true,
  },
  stacked: false,
  plugins: {
    zoom: {
      pan: {
        enabled: true,
      },
      zoom: {
        wheel: {
          enabled: true,
        },
        pinch: {
          enabled: true,
        },
        enabled: true,
      },
    },
    title: {
      display: true,
      text: "ORF Information",
    },
  },
};

const ProteinInfo = ({ seq }: { seq: AminoAcidData[] }) => {
  const data = {
    labels: seq.map((aad) => aad?.name || "-"),
    datasets: [
      {
        label: "Molecular Weight",
        barThickness: 10,
        data: seq.map((aad) => aad?.molecularWeight),
        borderColor: "rgb(255, 99, 132)",
        backgroundColor: "rgba(255, 99, 132, 0.5)",
        xAxisID: "y1",
      },
      {
        label: "pKa",
        barThickness: 10,
        data: seq.map((aad) => aad?.pKa),
        borderColor: "rgb(54, 162, 235)",
        backgroundColor: "rgba(54, 162, 235, 0.5)",
        xAxisID: "y2",
      },
      {
        label: "pKb",
        barThickness: 10,
        data: seq.map((aad) => aad?.pKb),
        borderColor: "rgb(75, 192, 192)",
        backgroundColor: "rgba(75, 192, 192, 0.5)",
        xAxisID: "y3",
      },
      {
        label: "pKx",
        barThickness: 10,
        data: seq.map((aad) => aad?.pKx),
        borderColor: "rgb(255, 205, 86)",
        backgroundColor: "rgba(255, 205, 86, 0.5)",
        xAxisID: "y4",
      },
      {
        label: "pl",
        barThickness: 10,
        data: seq.map((aad) => aad?.pl),
        borderColor: "rgb(153, 102, 255)",
        backgroundColor: "rgba(153, 102, 255, 0.5)",
        xAxisID: "y5",
      },
    ],
  };
  return (
    <div className="protein-info">
      <Bar options={options} data={data} />
    </div>
  );
};

export default ProteinInfo;
