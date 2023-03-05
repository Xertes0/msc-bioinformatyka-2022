import "./App.css";
import React, { useEffect, useState } from "react";

import SequenceInput from "./components/SequenceInput";
import AminoAcidView, {
  OpenReadingFrameViewData,
} from "./components/AminoAcidView";
import { ScrollContainer } from "react-indiana-drag-scroll";
import BioModuleLoad from "@cxx/biolib/bio.mjs";
import { data } from "./AminoAcidData";
import ProteinInfo from "./components/ProteinInfo";

export interface OpenReadingFrameIndicesInfo {
  nbStart: number;
  nbEnd: number;
  aaStart: number;
  aaEnd: number;
}

function App() {
  const [bioModule, setBioModule] = useState<null | BioModule>(null);
  const [sequence, setSequence] = useState("");

  useEffect(() => {
    BioModuleLoad().then((res) => {
      setBioModule(res);
    });
  }, []);

  const [orfData, setOrfData] = useState([[], [], []] as [
    OpenReadingFrameViewData[],
    OpenReadingFrameViewData[],
    OpenReadingFrameViewData[]
  ]);

  const [translatedSequences, setTranslatedSequences] = useState<
    [string, string, string]
  >([] as unknown as [string, string, string]);

  useEffect(() => {
    if (!bioModule) return;
    let result = bioModule.translate(sequence);
    setOrfData(
      result.map((result) => {
        let indices: OpenReadingFrameViewData[] = [];
        for (let i = 0; i < result.indices.size(); i++) {
          const { end, start } = result.indices.get(i);
          indices[i] = {
            sequence: Array.from(result.sequence.slice(start, end)).map(
              (c) => data.find((d) => d.symbol === c)?.abbreviation || "-"
            ),
            shift: start,
          };
        }
        return indices;
      }) as [
        OpenReadingFrameViewData[],
        OpenReadingFrameViewData[],
        OpenReadingFrameViewData[]
      ]
    );
    setTranslatedSequences(
      (result?.map((v) => v.sequence) as [string, string, string]) || []
    );
  }, [sequence, !!bioModule]);

  const [selectedOrfIndices, setSelectedOrfIndices] =
    useState<[number, number]>();
  const [selectedOrf, setSelectedOrf] = useState<string[]>();

  function submit(sequence: string) {
    setSequence(sequence);
  }

  return (
    <div className="App">
      <h1>Input a nucleic acid sequence:</h1>
      <SequenceInput onSubmit={submit} />
      <div className="form-description-container">
        <h2>The input accepts nucleobases mapped as follows:</h2>
        <ul>
          <li>A = Adenine</li>
          <li>C = Cytosine</li>
          <li>G = Guanine</li>
          <li>T = Thymine</li>
          <li>U = Uracil</li>
        </ul>
      </div>
      {sequence && (
        <ScrollContainer
          mouseScroll={true}
          hideScrollbars={true}
          className="aa-view-container"
        >
          <AminoAcidView
            sequence={Array.from(sequence)}
            sequences={
              translatedSequences.map((s) =>
                Array.from(s).map(
                  (c) => data.find((d) => d.symbol === c)?.abbreviation || "-"
                )
              ) as [string[], string[], string[]]
            }
            orfData={orfData}
            onOpenReadingFrameClick={({ idx }) => {
              setSelectedOrfIndices(idx);
              setSelectedOrf(orfData[idx[0]][idx[1]].sequence);
            }}
            selectedOrf={selectedOrfIndices}
          />
        </ScrollContainer>
      )}
      {selectedOrf && (
        <>
          <div className="protein-info-container">
            <ProteinInfo
              seq={selectedOrf?.map((s) =>
                data.find((data) => data.abbreviation === s)
              )}
            />
          </div>
        </>
      )}
    </div>
  );
}

export default App;
