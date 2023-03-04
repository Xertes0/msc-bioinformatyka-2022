import React, { SyntheticEvent, useEffect, useState } from "react";
import BioModuleLoad from "@cxx/biolib/bio.mjs";
import "./ProteinView.css"

export interface FoundProteinsViewProps {
  sequence: string;
  onProteinClick(event: ProteinClick): void;
}

export interface ProteinClick extends SyntheticEvent<HTMLTableRowElement> {
  target: HTMLTableRowElement;
  sequence: string;
}

function ProteinView({ onProteinClick }: FoundProteinsViewProps) {

  const [bioModule, setBioModule] = useState<null | BioModule>(null);

  useEffect(() => {
    BioModuleLoad().then((res) => {
      setBioModule(res);
    })
  }, []);

  //TODO: get data from props
  const sequence = "gatcgtcagatgtcagtacgtcagtcatgaacgcatgcgtacgacgtagcacgt".toUpperCase();
  const foundProteins = [[12, 20], [4, 23], [5, 12], [0, 54]]; // bioModule?.translate(sequence) || [];
  const highlightColors = foundProteins.map((_, i) => `hsl(${((i + 1) / foundProteins.length * 360) + 40}, 100%, 50%)`);
  const [activeProteinIdx, setActiveProteinIdx] = useState<undefined | number>(undefined);

  return (
    <table className="protein-view">
      <thead>
        <tr>
          {Array.from(sequence).map((c, i) => <th key={i}>{c}</th>)}
        </tr>
      </thead>
      <tbody>
      {
        foundProteins.map((t, i) => (
          <tr key={i} className={i == activeProteinIdx ? "active" : undefined} onClick={(event: React.MouseEvent<HTMLTableRowElement>) => {
            const newEvent = {...event, target: event.currentTarget as HTMLTableRowElement, sequence: sequence.slice(...t)};
            setActiveProteinIdx(newEvent.target.classList.contains("active") ? undefined : i);
            onProteinClick(newEvent);
          }}>
            {t[0] > 0 && <td colSpan={t[0]}></td>}
            {Array.from(sequence).slice(...t).map((c, ci) => <td key={ci} style={{ borderColor: highlightColors[i] }}>{c}</td>)}
          </tr>
        ))
      }
      </tbody>
    </table>
  )
}

export default ProteinView;