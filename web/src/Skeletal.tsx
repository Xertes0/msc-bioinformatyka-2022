// @ts-ignore
import BioModule from "@cxx/biolib/bio.mjs";
import {useEffect, useState} from "react";
import {useLocation} from "react-router-dom";

export default function Skeletal() {
    const search = useLocation().search;
    const searchParams = new URLSearchParams(search);
    const formula = searchParams.get("formula");
    // @ts-ignore
    const [formulaSvg, setFormulaSvg] = useState("");
    const [svgLoaded, setSvgLoaded] = useState(false);

    useEffect(() => {
        BioModule().then((res: any) => {
            setFormulaSvg(res.bio_draw_skeletal(formula));
            setSvgLoaded(true);
        })
    }, []);
    return (
        svgLoaded ?
            <div className={"Skeletal"}>
                <h1>Skeletal Formula</h1>
                <div id="svgDiv" style={{height: "45vh", maxWidth: "1000vw", margin: "auto", backgroundColor: "white"}}
                     dangerouslySetInnerHTML={{__html: formulaSvg}}>
                </div>
            </div> : <div>Loading...</div>
    )
}
