import "./App.css";
import React, { useState } from "react";

import SequenceInput from "./SequenceInput";
import ProteinView from "./ProteinView";
import {ScrollContainer} from "react-indiana-drag-scroll";

function App() {
    const [sequence, setSequence] = useState("");

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
            { sequence &&
                <ScrollContainer
                  mouseScroll={true}
                  hideScrollbars={true}
                  className="protein-view-container">
                    <ProteinView sequence={sequence} onOpenReadingFrameClick={() => {  }}/>
                </ScrollContainer>
            }

        </div>
    )
}

export default App;
