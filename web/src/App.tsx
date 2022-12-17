import "./App.css";
import React, { useEffect, useState } from "react";

import OpenReadingFrame from "./components/OpenReadingFrame";
import BioModuleLoad from "@cxx/biolib/bio.mjs";

function App() {
    const [bioModule, setBioModule] = useState<null | BioModule>(null);
    const [orfs, setOrfs] = useState([]);

    useEffect(() => {
        BioModuleLoad().then((res) => {
            setBioModule(res);
        })
    }, []);

    function orfClick(event: React.MouseEvent<HTMLDivElement>) {
        let element = event.target as HTMLDivElement;
        if (element.tagName != "A") {
            return;
        }
        window.open(`skeletal.html?formula=${element.innerText}`, '_blank', 'noopener noreferrer');
    }

    function transSubmit(event: React.FormEvent<HTMLFormElement>) {
        event.preventDefault();
        if (!bioModule) {
            return;
        }

        let form = event.target as HTMLFormElement;
        let input = form.children[0].children[1] as HTMLInputElement;
        let str = input.value;
        if (str == "") {
            return;
        }

        setOrfs(bioModule.translate(
            str,
            (index: Number) => {
                return [`<a id='proteinSeq${index}'>`, "</a>"];
            }
        )
        );
    }

    return (
        <div className="App">
            <div className="card">
                <h1>Translate a sequence</h1>
                {
                    bioModule &&
                    <div className="card">
                        <form onSubmit={transSubmit}>
                            <label>
                                Sequence<br></br>
                                <input type="text"></input>
                            </label>
                        </form>
                    </div>
                }
                <div onClick={orfClick}>
                    {
                        orfs.map((orf, index) => {
                            return <OpenReadingFrame key={index} id={index} content={orf} />
                        })
                    }
                </div>
            </div>
        </div>
    )
}

export default App;
