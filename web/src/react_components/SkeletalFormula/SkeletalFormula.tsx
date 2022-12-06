import {useParams} from "react-router-dom";
// @ts-ignore
import BioModule from "@cxx/biolib/bio.mjs";
import {useEffect, useState} from "react";

export default function SkeletalFormula() {
    const {formula} = useParams();
    // @ts-ignore
    const [bioModule, setBioModule] = useState({
        //bio_test() {
        //    return Number();
        //},
    });
    const [bioModuleLoaded, setBioModuleLoaded] = useState(false);

    useEffect(() => {
        setBioModuleLoaded(false);
        BioModule().then((res: any) => {
            setBioModuleLoaded(true);
            setBioModule(res);
        })
    }, []);
    return (
        bioModuleLoaded ?
            <div className={"SkeletalFormula"}>
                <h1>Skeletal Formula</h1>
                <div id="svgDiv" style={{height: "45vh", maxWidth: "1000vw", margin: "auto", backgroundColor: "white"}}
                     dangerouslySetInnerHTML={
                         function () {
                             let svg = document.createElement("svg");//"<svg width='100%' height='100%' id='aa_svg'" + " xmlns='http://www.w3.org/2000/svg'></svg>");// + bioModule.bio_draw_skeletal(formula) + "</svg>");
                             svg.setAttribute("width", "100%");
                             svg.setAttribute("height", "100%");
                             svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");
                             // @ts-ignore
                             svg.innerHTML = bioModule.bio_draw_skeletal(formula);
                             setTimeout(() => {
                                 // @ts-ignore
                                 let svg = document.getElementsByTagName("svg")[0];
                                 let bbox: SVGRect = svg.getBBox();
                                 // @ts-ignore
                                 svg.setAttribute("viewBox", `${bbox.x} ${bbox.y} ${bbox.width} ${bbox.height + bbox.y}`)
                             }, 100);
                             return {__html: svg.outerHTML};
                         }()
                     }>
                </div>
            </div> : <div>Loading...</div>
    )
}