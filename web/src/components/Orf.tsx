export default function Orf(porps: { id: number, content: string }) {
    return (
        <div id={`orf${porps.id}`} dangerouslySetInnerHTML={{__html: porps.content}} className="card" style={{
            border: "1px" +
                " solid" +
                " grey", marginBottom: "2px"
        }}>
        </div>
    )
}