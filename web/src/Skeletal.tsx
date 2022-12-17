import { useEffect, useState } from "react";
import { useLocation } from "react-router-dom";

import BioModuleLoad from "@cxx/biolib/bio.mjs";

export default function Skeletal() {
    const searchParams = new URLSearchParams(useLocation().search);
    const formula = searchParams.get("formula");
    if (!formula) {
        return <div>Supply `formula` in get parameters.</div>;
    } else if (formula.length < 1) {
        return <div>Supplied `formula` is too short.</div>;
    }

    const [formulaSvg, setFormulaSvg] = useState<string | null>(null);

    useEffect(() => {
        BioModuleLoad().then((res) => {
            setFormulaSvg(res.drawSkeletal(formula));
        })
    }, []);
    return (
        formulaSvg ?
            <div className={"Skeletal"}>
                <h1>Skeletal Formula</h1>
                <div id="svgDiv" style={{ height: "45vh", maxWidth: "1000vw", margin: "auto", backgroundColor: "white" }}
                    dangerouslySetInnerHTML={{ __html: formulaSvg }}>
                </div>
            </div> : <div>Loading...</div>
    )
}
